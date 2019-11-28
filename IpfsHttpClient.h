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

#ifndef IPFSHTTPCLIENT_H_
#define IPFSHTTPCLIENT_H_

#include "IpfsFiles.h"
#include "IpfsAddRsp.h"

class IpfsHttpClient
{
public:
	string uri; 
public:
	shared_ptr<IpfsAddRsp> add(const string& path, int& err, string& desc); 
	string get(const string& key, int& err, string& desc); 
public:
	shared_ptr<list<shared_ptr<IpfsFileLsRspEntry>>> filesLs(const string& path, int& err, string& desc); 
	string filesRead(const string& path, int& err, string& desc); 
	string filesRead(const string& path, ullong offset, long long len, int& err, string& desc); 
	bool filesRm(const string& path, int& err, string& desc); 
	bool filesRm(const string& path, bool recursive, bool force, int& err, string& desc); 
	shared_ptr<IpfsFileStatRsp> filesStat(const string& path, int& err, string& desc); 
	bool filesWrite(const string& path, uchar* dat, int len, int& err, string& desc); 
	bool filesWrite(const string& path, ullong offset, bool create, bool parents, bool truncate, uchar* dat, int len, int& err, string& desc); 
	bool filesWrite(const string& local, const string& path, int& err, string& desc); 
	bool filesWrite(const string& local, const string& path, ullong offset, bool create, bool parents, bool truncate, int& err, string& desc); 
public:
	string httpGet(const string& query, int& err, string& desc); 
public:
	IpfsHttpClient(const string uri);
	virtual ~IpfsHttpClient();
};

#endif 
