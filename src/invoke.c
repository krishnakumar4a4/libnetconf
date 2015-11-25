#include<stdio.h>
#include<stdlib.h>
#include "notifications.h"
#include "libnetconf.h"
#include "netconf.h"
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
struct nc_cpblts_dup* cpblts=NULL;
nc_rpc* x=NULL;
nc_rpc* y=NULL;
nc_rpc* z=NULL;
//extern struct nc_shared_info *nc_infon;
void nc_invoke()
{
char* p=NULL;
FILE *fp;
FILE *fp1;
FILE *fd4;
//char* c1="<?xml version=\"1.0\"?>\
          <rpc message-id=\"206\">\
           <edit-config>\
            <target><running/></target>\
             <config>\
              <econfig:contexts xmlns:econfig=\"urn:ietf:params:xml:ns:yang:ericsson-config\">\
               <econtext:context><econtext:context-name>test8</econtext:context-name></econtext:context>\
              </econfig:contexts>\
             </config>\
           </edit-config>\
          </rpc>";
//struct nc_session *session_n = NULL;
get_last_session();
//DBG("%lu",k);
//char* c="<?xml version=\"1.0\"?>\
  <methodCall>\
     <methodName>circleArea</methodName>\
           <params>\
                    <param>\
                                <value><double>2.41</double></value>\
                                         </param>\
                                               </params>\
                                               </methodCall>";
char* c="<?xml version=\"1.0\"?>\
         <rpc xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\" message-id=\"100\">\
        <get/>\
        </rpc>";
char* c1="<?xml version=\"1.0\"?>\
  <rpc message-id=\"206\" xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\" xmlns:einterface=\"urn:ietf:params:xml:ns:yang:ericsson-interface\" xmlns:if=\"urn:ietf:params:xml:ns:yang:ietf-interfaces\" xmlns:eethernet=\"urn:ietf:params:xml:ns:yang:ericsson-ethernet\" xmlns:econtext=\"urn:ietf:params:xml:ns:yang:ericsson-context\" xmlns:econfig=\"urn:ietf:params:xml:ns:yang:ericsson-config\" xmlns:ianaift=\"urn:ietf:params:xml:ns:yang:iana-if-type\">\
    <edit-config>\
     <target><running/></target>\
      <config xmlns:xc=\"urn:ietf:params:xml:ns:netconf:base:1.0\">\
     <econfig:contexts>\
       <econtext:context><econtext:context-name>test8</econtext:context-name></econtext:context>\
      </econfig:contexts>\
      <if:interfaces>\
           <if:interface>\
            <if:name>int1@test8</if:name>\
             <if:type>ianaift:ipForward</if:type>\
              <einterface:l3-interface>\
               <einterface:context>test8</einterface:context>\
                <einterface:ip>\
                 <einterface:address>\
                  <einterface:addr-primary>\
                   <einterface:addr>1.1.1.1/24</einterface:addr>\
                  </einterface:addr-primary>\
                 </einterface:address>\
                </einterface:ip>\
              </einterface:l3-interface>\
             </if:interface>\
             <if:interface>\
              <if:name>int2@test8</if:name>\
               <if:type>ianaift:ipForward</if:type>\
                <einterface:l3-interface>\
                 <einterface:context>test8</einterface:context>\
                  <einterface:ip>\
                   <einterface:address>\
                    <einterface:addr-primary>\
                     <einterface:addr>2.2.2.2/24</einterface:addr>\
                    </einterface:addr-primary>\
                   </einterface:address>\
                  </einterface:ip>\
                </einterface:l3-interface>\
             </if:interface>\
            </if:interfaces>\
        </config>\
       </edit-config>\
     </rpc>";
//char* c1="<?xml version=\"1.0\"?>\
            <rpc message-id=\"206\" xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\">\
            <edit-config>\
               <target><running/></target>\
                     <config xmlns:xc=\"urn:ietf:params:xml:ns:netconf:base:1.0\">\
                      <einterface:l3-interface xmlns:einterface=\"urn:ietf:params:xml:ns:yang:ericsson-interface\">\
                          <einterface:context>test8</einterface:context>\
                          <einterface:ip>\
                                <einterface:address>\
                                        <einterface:addr-primary>\
                                                  <einterface:addr>2.2.2.1/24</einterface:addr>\
                                        </einterface:addr-primary>\
                                </einterface:address>\
                           </einterface:ip>\
                      </einterface:l3-interface>\
                      </config>\
          </edit-config>\
        </rpc>";
//char* c1="<?xml version=\"1.0\"?>\
                      <rpc message-id=\"206\" xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\">\
                            <edit-config>\
                              <target><running/></target>\
                              <config xmlns:xc=\"urn:ietf:params:xml:ns:netconf:base:1.0\">\
                                    <if:interfaces xmlns:if=\"urn:ietf:params:xml:ns:yang:ietf-interfaces\">\
                                            <if:interface>\
                                                    <if:name>int2@test8</if:name>\
                                                    <if:type>ianaift:ipForward</if:type>\
                                            </if:interface>\
                                    </if:interfaces>\
                              </config>\
                            </edit-config>\
                      </rpc>";
//char* c2="
//x=nc_rpc_build(c1, NULL);
//int n=nc_rpc_get_op(x);
//nc_verb_verbose(nc_rpc_dump(x));
//print_dbg(n);
//y=ncds_apply_rpc2all(nc_session_dummy_k("krishna","pwd",NULL),x,NULL);
//nc_verb_verbose(nc_rpc_dump(y));
fp=fopen("checkget.txt","wr+");
fp1=fopen("checkeditted.txt","wr+");
cpblts=nc_session_get_cpblts_default();
z=np_ssh_client_netconf_rpc_k(nc_session_dummy_k("krishna","pwd",cpblts),c,cpblts);
//nc_verb_verbose(nc_rpc_dump(z));
fwrite(nc_rpc_dump(z),1,strlen(nc_rpc_dump(z)),fp);
fclose(fp);
fd4=fopen("replydata.txt","wr+");
p=nc_reply_get_data(z);
fwrite(p,1,strlen(p),fd4);
z=np_ssh_client_netconf_rpc_k(nc_session_dummy_k("krishna","pwd",cpblts),c1,cpblts);
nc_verb_verbose(nc_rpc_dump(z));
z=np_ssh_client_netconf_rpc_k(nc_session_dummy_k("krishna","pwd",cpblts),c,cpblts);
nc_verb_verbose(nc_rpc_dump(z));
fwrite(nc_rpc_dump(z),1,strlen(nc_rpc_dump(z)),fp1);
fclose(fp1);
//nc_session_recv_rpc(nc_session_dummy_k("krishna","pwd",NULL),100000, &x);
}
