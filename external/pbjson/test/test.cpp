/*
 * test.cpp
 *
 *  Created on: Feb 11, 2014
 *      Author: wangqiying
 */
#include "test.pb.h"
#include "pbjson.hpp"
#include <stdio.h>

int main()
{
    Request request;
    request.mutable_header()->set_user("test_user");
    request.mutable_header()->set_passwd("test_passwd");
    request.set_arg1(101);
    request.add_arg2(10100000000000001LL);
    request.add_arg2(1022222201LL);
    request.set_arg3(3.1415926);

    Arg* arg = request.add_args();
    arg->set_f1("xf1");
    arg->set_f2("xf2");
    arg = request.add_args();
    arg->set_f1("xff1");
    arg->set_f2("xff2");

    std::string str;
    pbjson::pb2json(&request, str);
    printf("PB2Json result:\n%s\n", str.c_str());

    Request new_request;
    std::string err;
    int ret = pbjson::json2pb(str, &new_request, err);
    printf("Json2PB result:\%d\n", ret);

    return 0;
}

