#include <map>
#include <iostream>
#include <emscripten/emscripten.h>
using namespace std;


#include "crypto.hpp"
#include "transaction.hpp"

// https://gist.github.com/WesThorburn/00c47b267a0e8c8431e06b14997778e4



#ifdef __cplusplus
extern "C" {
#endif

 inline const char* cstr(const std::string& message) {
    char * cstr = new char [message.length()+1];
    std::strcpy (cstr, message.c_str());
    return cstr;
}

EMSCRIPTEN_KEEPALIVE const char* generateKeyPair(int argc, char ** argv) {
    std::pair<PublicKey,PrivateKey> pair = generateKeyPair();
    PublicKey publicKey = pair.first;
    PrivateKey privateKey = pair.second;
    PublicWalletAddress w = walletAddressFromPublicKey(publicKey);
    string wallet = walletAddressToString(walletAddressFromPublicKey(publicKey));
    string pubKey = publicKeyToString(publicKey);
    string privKey = privateKeyToString(privateKey);
    json key;
    key["wallet"] = wallet;
    key["publicKey"] = pubKey;
    key["privateKey"] = privKey;
    return cstr(key.dump());
}

EMSCRIPTEN_KEEPALIVE const char* createTransaction(char* st) {
    string str = string(st);
    cout<<str<<endl;
    json data = json::parse(str);
    PublicWalletAddress from = stringToWalletAddress(data["from"]);
    PublicWalletAddress to = stringToWalletAddress(data["to"]);
    TransactionAmount amount = data["amount"];
    TransactionAmount fee = data["fee"];
    uint64_t timestamp = data["timestamp"];
    PublicKey pubKey = stringToPublicKey(data["publicKey"]);
    PrivateKey privKey = stringToPrivateKey(data["privateKey"]);
    
    Transaction t(from, to, amount, pubKey, fee, timestamp);
    t.sign(pubKey, privKey);
    return cstr(t.toJson().dump());
}


#ifdef __cplusplus
}
#endif

int main(int argc, char** argv) {
    return 0;
}
