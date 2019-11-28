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

#include <curl/curl.h>
#include "CurlOper.h"

size_t curl_rsp_header_cb(const char* ptr, size_t size, size_t nmemb, string* header)
{
	if (0 == size || NULL == ptr)
		return -1;
	header->append(ptr, nmemb * size);
	return nmemb * size;
}

size_t curl_rsp_cb(const char *ptr, size_t size, size_t nmemb, string* rsp)
{
	if (0 == size || NULL == ptr)
		return -1;
	rsp->append(ptr, nmemb * size);
	return nmemb * size;
}

CurlOper::CurlOper()
{

}

string CurlOper::httpGet(const string& url, int& err, string& desc)
{
	CURL* curl = ::curl_easy_init();
	::curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	::curl_easy_setopt(curl, CURLOPT_TIMEOUT, -1);
	string rspHeader;
	string rspBody;
	::curl_easy_setopt(curl, CURLOPT_WRITEHEADER, (void* ) &rspHeader);
	::curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, curl_rsp_header_cb);
	::curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void* ) &rspBody);
	::curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_rsp_cb);
	CURLcode res = ::curl_easy_perform(curl);
	long code;
	::curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
	if (res != CURLE_OK)
	{
		err = res;
		desc.assign(::curl_easy_strerror(res));
		::curl_easy_cleanup(curl);
		return "";
	}
	::curl_easy_cleanup(curl);
	err = (code == 200 ? 0 : code);
	desc.clear();
	return rspBody;
}

bool CurlOper::httpFormData(const string& url, const string& fileName, uchar* dat, long len, int& err, string& desc)
{
	string fn = CurlOper::urlEncode(fileName);
	struct curl_httppost* post = NULL;
	struct curl_httppost* last = NULL;
	::curl_formadd(&post, &last, CURLFORM_COPYNAME, "file", CURLFORM_BUFFER, fn.c_str(), CURLFORM_BUFFERPTR, dat, CURLFORM_BUFFERLENGTH, len, CURLFORM_CONTENTTYPE, "application/octet-stream", CURLFORM_END);
	CURL* curl = ::curl_easy_init();
	::curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	::curl_easy_setopt(curl, CURLOPT_TIMEOUT, -1);
	::curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
	CURLcode res = ::curl_easy_perform(curl);
	long code;
	::curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
	if (res != CURLE_OK)
	{
		err = res;
		desc.assign(::curl_easy_strerror(res));
		::curl_easy_cleanup(curl);
		return false;
	}
	::curl_easy_cleanup(curl);
	err = (code == 200 ? 0 : code);
	desc.clear();
	return err == 0;
}

bool CurlOper::httpFormData(const string& url, const string& filePath, const string& fileName, int& err, string& desc)
{
	string fn = CurlOper::urlEncode(fileName);
	struct curl_httppost* post = NULL;
	struct curl_httppost* last = NULL;
	::curl_formadd(&post, &last, CURLFORM_COPYNAME, "file", CURLFORM_BUFFER, fn.c_str(), CURLFORM_FILE, filePath.c_str(), CURLFORM_CONTENTTYPE, "application/octet-stream", CURLFORM_END);
	CURL* curl = ::curl_easy_init();
	::curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	::curl_easy_setopt(curl, CURLOPT_TIMEOUT, -1);
	::curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
	CURLcode res = ::curl_easy_perform(curl);
	long code;
	::curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
	if (res != CURLE_OK)
	{
		err = res;
		desc.assign(::curl_easy_strerror(res));
		::curl_easy_cleanup(curl);
		return false;
	}
	::curl_easy_cleanup(curl);
	err = (code == 200 ? 0 : code);
	desc.clear();
	return err == 0;
}

string CurlOper::httpFormData(const string& url, const string& filePath, int& err, string& desc)
{
	struct curl_httppost* post = NULL;
	struct curl_httppost* last = NULL;
	::curl_formadd(&post, &last, CURLFORM_COPYNAME, "file", CURLFORM_BUFFER, "", CURLFORM_FILE, filePath.c_str(), CURLFORM_CONTENTTYPE, "application/octet-stream", CURLFORM_END);
	string rspBody;
	CURL* curl = ::curl_easy_init();
	::curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	::curl_easy_setopt(curl, CURLOPT_TIMEOUT, -1);
	::curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
	::curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void* ) &rspBody);
	::curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_rsp_cb);
	CURLcode res = ::curl_easy_perform(curl);
	long code;
	::curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
	if (res != CURLE_OK)
	{
		err = res;
		desc.assign(::curl_easy_strerror(res));
		::curl_easy_cleanup(curl);
		return "";
	}
	::curl_easy_cleanup(curl);
	err = (code == 200 ? 0 : code);
	desc.clear();
	return rspBody;
}

string CurlOper::urlEncode(const string& url)
{
	CURL* curl = ::curl_easy_init();
	char* ret = ::curl_easy_escape(curl, url.c_str(), url.length());
	string str(ret);
	::curl_free(ret);
	::curl_easy_cleanup(curl);
	return str;
}

CurlOper::~CurlOper()
{

}

