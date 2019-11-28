/*
  Copyright 2019 www.dev5.cn, Inc. dev5@qq.com
 
  This file is part of X-MSG-IM.
 
  X-MSG-IM is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  X-MSG-IM is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU Affero General Public License
  along with X-MSG-IM.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <archive.h>
#include "IpfsHttpClient.h"
#include "IpfsFiles.h"
#include "CurlOper.h"
#include "json.hpp"
using namespace nlohmann;

IpfsHttpClient::IpfsHttpClient(const string uri)
{
	this->uri = uri;
}

shared_ptr<IpfsAddRsp> IpfsHttpClient::add(const string& path, int& err, string& desc)
{
	string str;
	SPRINTF_STRING(&str, "%sadd", this->uri.c_str())
	string rsp = CurlOper::httpFormData(str, path, err, desc);
	if (err != 0)
		return nullptr;
	json j = json::parse(rsp, nullptr, false);
	shared_ptr<IpfsAddRsp> iar(new IpfsAddRsp());
	auto iter = j.find("Name");
	if (iter == j.end() || !iter->is_string())
	{
		LOG_ERROR("can not found `Name`, rsp: %s", rsp.c_str())
		return nullptr;
	}
	iar->name = iter.value().get<string>();
	iter = j.find("Hash");
	if (iter == j.end() || !iter->is_string())
	{
		LOG_ERROR("can not found `Hash`, rsp: %s", rsp.c_str())
		return nullptr;
	}
	iar->hash = iter.value().get<string>();
	iter = j.find("Size");
	if (iter == j.end() || !iter->is_string())
	{
		LOG_ERROR("can not found `Size`, rsp: %s", rsp.c_str())
		return nullptr;
	}
	iar->size = iter.value().get<string>();
	return iar;
}

string IpfsHttpClient::get(const string& key, int& err, string& desc)
{
	string str;
	SPRINTF_STRING(&str, "%sget?arg=%s&archive=false&compress=true&compress-level=5", this->uri.c_str(), CurlOper::urlEncode(key.c_str()).c_str())
	string rsp = CurlOper::httpGet(str, err, desc);
	if (err != 0)
		return rsp;
	struct archive* a = archive_read_new();
	archive_read_support_filter_gzip(a);
	archive_read_support_format_zip(a);
	archive_read_support_format_raw(a);
	int ret = archive_read_open_memory(a, rsp.data(), rsp.length());
	if (ret != ARCHIVE_OK)
	{
		err = 1;
		desc = "uncompress object data failed";
		return "";
	}
	archive_entry* ae;
	ret = ::archive_read_next_header(a, &ae);
	if (ret != ARCHIVE_OK)
	{
		err = 1;
		desc = "uncompress object data failed";
		return "";
	}
	uchar buf[0x8000];
	string dat;
	while (1)
	{
		int size = ::archive_read_data(a, buf, 0x8000);
		if (size < 0)
		{
			err = 1;
			desc = "uncompress object data failed";
			return "";
		}
		if (size == 0)
			break;
		dat.append((const char*) buf, size);
	}
	::archive_read_free(a);
	return dat;
}

shared_ptr<list<shared_ptr<IpfsFileLsRspEntry>>> IpfsHttpClient::filesLs(const string& path, int& err, string& desc)
{
	return IpfsFiles::filesLs(this, path, err, desc);
}

string IpfsHttpClient::filesRead(const string& path, int& err, string& desc)
{
	return IpfsFiles::filesRead(this, path, 0, numeric_limits<long long>::max(), err, desc);
}

string IpfsHttpClient::filesRead(const string& path, ullong offset, long long len, int& err, string& desc)
{
	return IpfsFiles::filesRead(this, path, offset, len, err, desc);
}

bool IpfsHttpClient::filesRm(const string& path, int& err, string& desc)
{
	return IpfsFiles::filesRm(this, path, false, false, err, desc);
}

bool IpfsHttpClient::filesRm(const string& path, bool recursive, bool force, int& err, string& desc)
{
	return IpfsFiles::filesRm(this, path, recursive, force, err, desc);
}

shared_ptr<IpfsFileStatRsp> IpfsHttpClient::filesStat(const string& path, int& err, string& desc)
{
	return IpfsFiles::filesStat(this, path, err, desc);
}

bool IpfsHttpClient::filesWrite(const string& path, uchar* dat, int len, int& err, string& desc)
{
	return IpfsFiles::filesWrite(this, path, 0, true, false, false, dat, len, err, desc);
}

bool IpfsHttpClient::filesWrite(const string& path, ullong offset, bool create, bool parents, bool truncate, uchar* dat, int len, int& err, string& desc)
{
	return IpfsFiles::filesWrite(this, path, offset, create, parents , truncate, dat, len, err, desc);
}

bool IpfsHttpClient::filesWrite(const string& local, const string& path, int& err, string& desc)
{
	return IpfsFiles::filesWrite(this, local, path, 0, true, false, false, err, desc);
}

bool IpfsHttpClient::filesWrite(const string& local, const string& path, ullong offset, bool create, bool parents, bool truncate, int& err, string& desc)
{
	return IpfsFiles::filesWrite(this, local, path, offset, create, parents, truncate, err, desc);
}

string IpfsHttpClient::httpGet(const string& query, int& err, string& desc)
{
	return CurlOper::httpGet(this->uri + query, err, desc);
}

IpfsHttpClient::~IpfsHttpClient()
{

}

