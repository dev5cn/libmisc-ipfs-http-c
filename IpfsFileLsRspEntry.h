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

#ifndef IPFSFILELSRSPENTRY_H_
#define IPFSFILELSRSPENTRY_H_

#include <libmisc.h>

class IpfsFileLsRspEntry
{
public:
	string name;
	ullong type;
	ullong size;
	string hash;
public:
	string toString();
	IpfsFileLsRspEntry();
	virtual ~IpfsFileLsRspEntry();
};

#endif 