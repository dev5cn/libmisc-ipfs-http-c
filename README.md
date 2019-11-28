# libmisc-ipfs-http-c

* 这是一个建立在IPFS HTTP API上的c++ client library. 已实现的功能包括:

	* files.ls
	* files.read
	* files.rm
	* files.stat
	* files.write
	* add
	* get
	* 一个适用于http-get的通用接口.

* demo:

```cpp
#include <libmisc-ipfs-http-c.h>

int main(int argc, char **argv)
{
	Log::setTrace();
	shared_ptr<IpfsHttpClient> ipfs(new IpfsHttpClient("http://127.0.0.1:5001/api/v0/"));
	int err;
	string desc;
	//
	ipfs->filesWrite("/home/xzwdev/ipfs-hello-world0.txt", "/ipfs-hello-world0.txt", err, desc); /* ipfs files write. */
	ipfs->filesWrite("/home/xzwdev/ipfs-hello-world1.txt", "/ipfs-hello-world1.txt", err, desc);
	shared_ptr<list<shared_ptr<IpfsFileLsRspEntry>>> lis = ipfs->filesLs("/", err, desc);
	for (auto& it : *lis)
	{
		LOG_DEBUG("%s", it->toString().c_str())
	}
	LOG_DEBUG("content: %s", ipfs->filesRead("/ipfs-hello-world0.txt", err, desc).c_str()) /* ipfs files read. */
	ipfs->filesRm("/ipfs-hello-world0.txt", err, desc); /* ipfs files rm */
	LOG_DEBUG("stat: %s", ipfs->filesStat("/ipfs-hello-world1.txt", err, desc)->toString().c_str()) /* ipfs files stat. */
	shared_ptr<IpfsAddRsp> add = ipfs->add("/home/xzwdev/ipfs-hello-world0.txt", err, desc);
	LOG_DEBUG("add: %s", add->toString().c_str()) /* ipfs add. */
	LOG_DEBUG("get: %s", ipfs->get(add->hash, err, desc).c_str()) /* ipfs get. */
	//
	LOG_DEBUG("%s", ipfs->httpGet("swarm/peers", err, desc).c_str()) /* ipfs xxx. */
	return EXIT_SUCCESS;
}
```

* 依赖

	* [libmisc](https://github.com/dev5cn/libmisc-cpp)
	* [libcurl](https://github.com/curl/curl)
	* [libarchive](https://github.com/libarchive/libarchive)
	
* 编译

```js
export PLAT=LINUX
export LIB_CURL=/home/dev5/tools/curl-7.66.0
export LIB_ARCHIVE=/home/dev5/tools/libarchive-3.4.0
export CC=/usr/bin/gcc
export CXX=/usr/bin/g++
export CXX_FLAG="-g3 -O3"
export MAKE_J="-j3"
```

```js
ls ./
libmisc-cpp libmisc-ipfs-http-c
chmod 775 */*.sh
cd libmisc-ipfs-http-c;./build.sh
```

	