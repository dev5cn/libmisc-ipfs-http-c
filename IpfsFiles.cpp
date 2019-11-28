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

#include "IpfsFiles.h"
#include "IpfsHttpClient.h"
#include "CurlOper.h"
#include "json.hpp"
using namespace nlohmann;

IpfsFiles::IpfsFiles()
{

}

shared_ptr<list<shared_ptr<IpfsFileLsRspEntry>>> IpfsFiles::filesLs(class IpfsHttpClient* ipfs, const string& path, int& err, string& desc)
{
	string str;
	SPRINTF_STRING(&str, "%s%s/ls?arg=%s&l=true&u=true&encoding=json", ipfs->uri.c_str(), "files", CurlOper::urlEncode(path).c_str())
	string rsp = CurlOper::httpGet(str, err, desc);
	if (err != 0)
	{
		LOG_DEBUG("execute http get failed, url: %s, err: %d, desc: %s", str.c_str(), err, desc.c_str())
		return nullptr;
	}
	json j = json::parse(rsp, nullptr, false);
	auto it = j.find("Entries");
	if (it == j.end() || !it.value().is_array())
	{
		LOG_ERROR("can not found `Entries`, rsp: %s", rsp.c_str())
		return nullptr;
	}
	shared_ptr<list<shared_ptr<IpfsFileLsRspEntry>>> lis(new list<shared_ptr<IpfsFileLsRspEntry>>());
	for (size_t i = 0; i < it.value().size(); ++i)
	{
		auto& entry = it.value().at(i);
		shared_ptr<IpfsFileLsRspEntry> e(new IpfsFileLsRspEntry());
		auto iter = entry.find("Name");
		if (iter == entry.end() || !iter->is_string())
		{
			LOG_ERROR("can not found `Name`, rsp: %s", rsp.c_str())
			return nullptr;
		}
		e->name = iter.value().get<string>();
		iter = entry.find("Type");
		if (iter == entry.end() || !iter->is_number())
		{
			LOG_ERROR("can not found `Type`, rsp: %s", rsp.c_str())
			return nullptr;
		}
		e->type = iter.value().get<ullong>();
		iter = entry.find("Size");
		if (iter == entry.end() || !iter->is_number())
		{
			LOG_ERROR("can not found `Size`, rsp: %s", rsp.c_str())
			return nullptr;
		}
		e->size = iter.value().get<ullong>();
		iter = entry.find("Hash");
		if (iter == entry.end() || !iter->is_string())
		{
			LOG_ERROR("can not found `Hash`, rsp: %s", rsp.c_str())
			return nullptr;
		}
		e->hash = iter.value().get<string>();
		lis->push_back(e);
	}
	return lis;
}

string IpfsFiles::filesRead(class IpfsHttpClient* ipfs, const string& path, ullong offset, long long count, int& err, string& desc)
{
	string str;
	SPRINTF_STRING(&str, "%s%s/read?arg=%s&offset=%llu&count=%lld", ipfs->uri.c_str(), "files", CurlOper::urlEncode(path).c_str(), offset, count)
	return CurlOper::httpGet(str, err, desc);
}

bool IpfsFiles::filesRm(class IpfsHttpClient* ipfs, const string& path, bool recursive, bool force, int& err, string& desc)
{
	string str;
	SPRINTF_STRING(&str, "%s%s/rm?arg=%s&recursive=%s&force=%s", ipfs->uri.c_str(), "files", CurlOper::urlEncode(path).c_str(), recursive ? "true" : "false", force ? "true" : "false")
	CurlOper::httpGet(str, err, desc);
	return err == 0;
}

shared_ptr<IpfsFileStatRsp> IpfsFiles::filesStat(class IpfsHttpClient* ipfs, const string& path, int& err, string& desc)
{
	string str;
	SPRINTF_STRING(&str, "%s%s/stat?arg=%s&encoding=json", ipfs->uri.c_str(), "files", CurlOper::urlEncode(path).c_str())
	string rsp = CurlOper::httpGet(str, err, desc);
	if (err != 0)
	{
		LOG_DEBUG("execute http get failed, url: %s, err: %d, desc: %s", str.c_str(), err, desc.c_str())
		return nullptr;
	}
	json j = json::parse(rsp, nullptr, false);
	shared_ptr<IpfsFileStatRsp> e(new IpfsFileStatRsp());
	auto iter = j.find("Hash");
	if (iter == j.end() || !iter->is_string())
	{
		LOG_ERROR("can not found `Hash`, rsp: %s", rsp.c_str())
		return nullptr;
	}
	e->hash = iter.value().get<string>();
	iter = j.find("Size");
	if (iter == j.end() || !iter->is_number())
	{
		LOG_ERROR("can not found `Size`, rsp: %s", rsp.c_str())
		return nullptr;
	}
	e->size = iter.value().get<ullong>();
	iter = j.find("CumulativeSize");
	if (iter == j.end() || !iter->is_number())
	{
		LOG_ERROR("can not found `CumulativeSize`, rsp: %s", rsp.c_str())
		return nullptr;
	}
	e->cumulativeSize = iter.value().get<ullong>();
	iter = j.find("Blocks");
	if (iter == j.end() || !iter->is_number())
	{
		LOG_ERROR("can not found `Blocks`, rsp: %s", rsp.c_str())
		return nullptr;
	}
	e->blocks = iter.value().get<ullong>();
	iter = j.find("Type");
	if (iter == j.end() || !iter->is_string())
	{
		LOG_ERROR("can not found `Type`, rsp: %s", rsp.c_str())
		return nullptr;
	}
	e->type = iter.value().get<string>();
	return e;
}

bool IpfsFiles::filesWrite(class IpfsHttpClient* ipfs, const string& path, ullong offset, bool create, bool parents, bool truncate, uchar* dat, int len, int& err, string& desc)
{
	string str;
	SPRINTF_STRING(&str, "%s%s/write?arg=%s&offset=%llu&create=%s&parents=%s&truncate=%s", ipfs->uri.c_str(), "files", CurlOper::urlEncode(path).c_str(), 
			offset,
			create ? "true" : "false",
			parents ? "true" : "false",
			truncate ? "true" : "false")
	string::size_type st = path.find_last_of("/");
	string fileName = (st == string::npos ? path : path.substr(st + 1, path.length()));
	return CurlOper::httpFormData(str, fileName, dat, (long) len, err, desc);
}

bool IpfsFiles::filesWrite(class IpfsHttpClient* ipfs, const string& local, const string& path, ullong offset, bool create, bool parents, bool truncate, int& err, string& desc)
{
	string str;
	SPRINTF_STRING(&str, "%s%s/write?arg=%s&offset=%llu&create=%s&parents=%s&truncate=%s", ipfs->uri.c_str(), "files", CurlOper::urlEncode(path).c_str(), 
			offset,
			create ? "true" : "false",
			parents ? "true" : "false",
			truncate ? "true" : "false")
	string::size_type st = path.find_last_of("/");
	string fileName = (st == string::npos ? path : path.substr(st + 1, path.length()));
	return CurlOper::httpFormData(str, local, fileName, err, desc);
}

IpfsFiles::~IpfsFiles()
{

}

