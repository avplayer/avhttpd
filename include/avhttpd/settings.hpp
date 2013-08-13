//
// settings.hpp
// ~~~~~~~~~~~~
//
// Copyright (c) 2013 Jack (jack dot wgm at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// path LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef __AVHTTPD_SETTINGS_HPP__
#define __AVHTTPD_SETTINGS_HPP__

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <vector>
#include <map>
#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/date_time.hpp>

namespace avhttpd {

// 常用有以下http选项.
namespace http_options {

	// 下面为avhttp内定的一些选项.
	static const std::string request_method("_request_method"); // 请求方式(GET/POST)
	static const std::string request_uri("_request_uri"); // 请求地址
	static const std::string http_version("_http_version");		// HTTP/1.0|HTTP/1.1
	static const std::string request_body("_request_body");		// 一般用于POST一些数据如表单之类时使用.
	static const std::string status_code("_status_code");	// HTTP状态码.
	static const std::string path("_path");		// 请求的path, 如http://abc.ed/v2/cma.txt中的/v2/cma.txt.
	static const std::string url("_url");		// 在启用keep-alive的时候, 请求host上不同的url时使用.
	// 以下是常用的标准http head选项.
	static const std::string host("Host");
	static const std::string accept("Accept");
	static const std::string range("Range");
	static const std::string cookie("Cookie");
	static const std::string referer("Referer");
	static const std::string user_agent("User-Agent");
	static const std::string content_type("Content-Type");
	static const std::string content_length("Content-Length");
	static const std::string content_range("Content-Range");
	static const std::string connection("Connection");
	static const std::string proxy_connection("Proxy-Connection");
	static const std::string accept_encoding("Accept-Encoding");
	static const std::string transfer_encoding("Transfer-Encoding");
	static const std::string content_encoding("Content-Encoding");

} // namespace http_options


// 具体的http的option选项实现.

class option
{
public:
	// 定义option_item类型.
	typedef std::pair<std::string, std::string> option_item;
	// 定义option_item_list类型.
	typedef std::vector<option_item> option_item_list;
	// for boost::assign::insert
	typedef option_item value_type;
public:
	option() {}
	~option() {}

public:

	// 这样就允许这样的应用:
	// http_stream s;
	// s.request_options(request_opts()("cookie","XXXXXX"));
	option & operator()(const std::string &key, const std::string &val)
	{
		insert(key, val);
		return *this;
	}

	// 添加选项, 由key/value形式添加.
	void insert(const std::string &key, const std::string &val)
	{
		m_opts.push_back(option_item(key, val));
	}

	// 添加选项，由 std::part 形式.
	void insert(value_type & item)
	{
		m_opts.push_back(item);
	}

	// 删除选项.
	void remove(const std::string &key)
	{
		for (option_item_list::iterator i = m_opts.begin(); i != m_opts.end(); i++)
		{
			if (i->first == key)
			{
				m_opts.erase(i);
				return;
			}
		}
	}

	// 查找指定key的value.
	bool find(const std::string &key, std::string &val) const
	{
		std::string s = key;
		boost::to_lower(s);
		for (option_item_list::const_iterator f = m_opts.begin(); f != m_opts.end(); f++)
		{
			std::string temp = f->first;
			boost::to_lower(temp);
			if (temp == s)
			{
				val = f->second;
				return true;
			}
		}
		return false;
	}

	// 查找指定的 key 的 value. 没找到返回 ""，　这是个偷懒的帮助.
	std::string find(const std::string & key) const
	{
		std::string v;
		find(key,v);
		return v;
	}

	// 得到Header字符串.
	std::string header_string() const
	{
		std::string str;
		for (option_item_list::const_iterator f = m_opts.begin(); f != m_opts.end(); f++)
		{
			if (!f->first.empty())
				if (f->first[0] != '_')
					str += (f->first + ": " + f->second + "\r\n");
		}
		return str;
	}

	// 清空.
	void clear()
	{
		m_opts.clear();
	}

	// 返回所有option.
	option_item_list& option_all()
	{
		return m_opts;
	}

	// 返回当前option个数.
	int size() const
	{
		return m_opts.size();
	}

protected:
	option_item_list m_opts;
};

// 请求时的http选项.
// _http_version, 取值 "HTTP/1.0" / "HTTP/1.1", 默认为"HTTP/1.1".
// _request_method, 取值 "GET/POST/HEAD", 默认为"GET".
// _request_body, 请求中的body内容, 取值任意, 默认为空.
// Host, 取值为http服务器, 默认为http服务器.
// Accept, 取值任意, 默认为"*/*".
// 这些比较常用的选项被定义在http_options中.
typedef option request_opts;

// http服务器返回的http选项.
// 一般会包括以下几个选项:
// _status_code, http返回状态.
// Server, 服务器名称.
// Content-Length, 数据内容长度.
// Connection, 连接状态标识.
typedef option response_opts;

// 一些默认的值.
static const int default_request_piece_num = 10;
static const int default_time_out = 11;
static const int default_piece_size = 32768;
static const int default_connections_limit = 5;
static const int default_buffer_size = 1024;



} // namespace avhttpd

#endif // __AVHTTPD_SETTINGS_HPP__
