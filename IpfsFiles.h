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

#ifndef IPFSFILES_H_
#define IPFSFILES_H_

#include "IpfsFileLsRspEntry.h"
#include "IpfsFileStatRsp.h"

class IpfsHttpClient;

class IpfsFiles
{
public:
	static shared_ptr<list<shared_ptr<IpfsFileLsRspEntry>>> filesLs(class IpfsHttpClient* ipfs, const string& path, int& err, string& desc); 
	static string filesRead(class IpfsHttpClient* ipfs, const string& path, ullong offset, long long count, int& err, string& desc); 
	static bool filesRm(class IpfsHttpClient* ipfs, const string& path, bool recursive, bool force, int& err, string& desc); 
	static shared_ptr<IpfsFileStatRsp> filesStat(class IpfsHttpClient* ipfs, const string& path, int& err, string& desc); 
	static bool filesWrite(class IpfsHttpClient* ipfs, const string& path, ullong offset, bool create, bool parents, bool truncate, uchar* dat, int len, int& err, string& desc); 
	static bool filesWrite(class IpfsHttpClient* ipfs, const string& local, const string& path, ullong offset, bool create, bool parents, bool truncate, int& err, string& desc); 
private:
	IpfsFiles();
	virtual ~IpfsFiles();
};

#endif 
