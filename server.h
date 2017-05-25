#pragma once
#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

extern Trie t;

#include "record.h"
#include "trie.h"
#include "trie_exceptions.h"

using boost::asio::ip::tcp;

class tcp_connection
	: public boost::enable_shared_from_this<tcp_connection> {
public:
	typedef boost::shared_ptr<tcp_connection> pointer;

	static pointer create(boost::asio::io_service& io_service) {
		return pointer(new tcp_connection(io_service));
	}

	tcp::socket& socket() {
		return socket_;
	}

	void start() {
		uint8_t charbuf[1200];
		int readbytes = socket_.receive(boost::asio::buffer(charbuf, 1200));

		Tins::DNS dns;
		dns = Tins::DNS(charbuf + 2, readbytes - 2);

		if (dns.type() == Tins::DNS::QUERY) {
			std::string queryName;
			std::vector<Record> results;
			for (const auto& query : dns.queries()) {
				queryName = query.dname();
				std::cout << queryName << "\n";
				std::transform(queryName.begin(), queryName.end(), queryName.begin(), ::toupper);

				// having a domain name with less than 3 characters in it is impossible, so return NXDOMAIN
				if (queryName.length() < 3) {
					dns.type(Tins::DNS::RESPONSE);
					dns.recursion_available(0);
					dns.rcode(3);
				}

				// this program does not support SRV records - return NotImp
				if (query.query_type() == Tins::DNS::SRV) {
					dns.type(Tins::DNS::RESPONSE);
					dns.recursion_available(0);
					dns.rcode(4);
				}

				try {
					results = t.lookup(queryName, query.query_type(), t.getRoot(), 0);
				} catch (nxdomainException &e) {
					// send a response with the NXDOMAIN rcode
					dns.type(Tins::DNS::RESPONSE);
					dns.recursion_available(0);
					dns.rcode(3);
				} catch (std::exception &e) {
					e.what();
				}

				// no results, so return empty answer with SOA record if it exists
				// rcode = 0 (NOERROR) with 0 answers
				if (results.size() == 0) {
					results = t.lookup(queryName, Tins::DNS::SOA, t.getRoot(), 0);

					for (std::vector<Record>::iterator it = results.begin(); it != results.end(); it++) {
						Tins::DNS::soa_record s = Tins::DNS::soa_record(
							(*it).getContent(),
							(*it).getMail(),
							(*it).getSerial(),
							(*it).getRefresh(),
							(*it).getRetry(),
							(*it).getExpire(),
							(*it).getMinTtl()
						);
						Tins::DNS::resource r = Tins::DNS::resource(
							(*it).getName(),
							(*it).getContent(),
							Tins::DNS::SOA,
							query.query_class(),
							(*it).getTtl()
						);
						r.data(s);
						dns.add_authority(r);
						// dns.authority_count();
					}

					dns.type(Tins::DNS::RESPONSE);
					dns.recursion_available(0);
					dns.rcode(0);
				}

				for (std::vector<Record>::iterator it = results.begin(); it != results.end(); it++) {
					if (query.query_type() == Tins::DNS::MX) {
						Tins::DNS::resource r = Tins::DNS::resource(
							(*it).getName(),
							(*it).getContent(),
							Tins::DNS::MX,
							query.query_class(),
							(*it).getTtl(),
							(*it).getPriority()
						);
						dns.add_answer(r);
					} else if (query.query_type() == Tins::DNS::SOA) {
						// create an SOA record first to get the data out of it
						Tins::DNS::soa_record s = Tins::DNS::soa_record(
							(*it).getContent(),
							(*it).getMail(),
							(*it).getSerial(),
							(*it).getRefresh(),
							(*it).getRetry(),
							(*it).getExpire(),
							(*it).getMinTtl()
						);
						Tins::DNS::resource r = Tins::DNS::resource(
							(*it).getName(),
							(*it).getContent(),
							Tins::DNS::SOA,
							query.query_class(),
							(*it).getTtl()
						);
						r.data(s);
						dns.add_answer(r);
					} else {
						Tins::DNS::resource r = Tins::DNS::resource(
							(*it).getName(),
							(*it).getContent(),
							query.query_type(),
							query.query_class(),
							(*it).getTtl()
						);
						dns.add_answer(r);
					}
				}

				if (dns.answers_count() > 0) {
					dns.type(Tins::DNS::RESPONSE);
					dns.recursion_available(0);
				}

				Tins::PDU::serialization_type s = dns.serialize();
				std::vector<uint8_t> data;

				uint8_t bLo = s.size() & 0xff;
				uint8_t bHi = (s.size() >> 8);

				data.push_back(bHi);
				data.push_back(bLo);

				data.insert(std::end(data), std::begin(s), std::end(s));

				boost::asio::async_write(socket_, boost::asio::buffer(data), boost::bind(&tcp_connection::handle_write, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
			}
		} 
}

private:
	tcp_connection(boost::asio::io_service& io_service)
		: socket_(io_service) {
	}

	void handle_write(const boost::system::error_code& /*error*/,
		size_t /*bytes_transferred*/) {
	}

	tcp::socket socket_;
	std::string message_;
};

class tcp_server {
public:
	tcp_server(boost::asio::io_service& io_service)
		: acceptor_(io_service, tcp::endpoint(tcp::v4(), 53)) {
		start_accept();
	}

private:
	void start_accept() {
		tcp_connection::pointer new_connection =
			tcp_connection::create(acceptor_.get_io_service());

		acceptor_.async_accept(new_connection->socket(),
			boost::bind(&tcp_server::handle_accept, this, new_connection,
				boost::asio::placeholders::error));
	}

	void handle_accept(tcp_connection::pointer new_connection,
		const boost::system::error_code& error) {
		if (!error)
		{
			new_connection->start();
		}

		start_accept();
	}

	tcp::acceptor acceptor_;
};