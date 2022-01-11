
#include "crypto.hpp"
#include <iostream>
#include <random>
#include <vector>
#include <string>
#include <array>
#include "ed25519/ed25519.h"
#include "openssl/sha.h"
 #include "openssl/ripemd.h"
using namespace std;

SHA256Hash SHA256(const char* buffer, size_t len) {
    std::array<uint8_t, 32> ret;
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, buffer, len);
    SHA256_Final(ret.data(), &sha256);
    return ret;
}

RIPEMD160Hash RIPEMD160(const char* buffer, size_t len) {
    RIPEMD160Hash ret;
    RIPEMD160_CTX ripemd;
    RIPEMD160_Init(&ripemd);
    RIPEMD160_Update(&ripemd, buffer, len);
    RIPEMD160_Final((unsigned char*)ret.data(), &ripemd);
    return ret;
}

SHA256Hash SHA256(string str) {
    return SHA256(str.c_str(), str.length());
}

std::vector<uint8_t> hexDecode(const string& hex) {
  vector<uint8_t> bytes;
  for (size_t i = 0; i < hex.length(); i += 2) {
    string byteString = hex.substr(i, 2);
    char byte = (char) strtol(byteString.c_str(), NULL, 16);
    bytes.push_back(byte);
  }
  return std::move(bytes);
}

string hexEncode(const char* buffer, size_t len) {
    static const char* const lut = "0123456789ABCDEF";
    std::string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i) {
        const unsigned char c = buffer[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    return output;
}

SHA256Hash stringToSHA256(string hex) {
    vector<uint8_t> bytes = hexDecode(hex);
    SHA256Hash sha;
    std::move(bytes.begin(), bytes.begin()+sha.size(), sha.begin());
    return sha;
}

string SHA256toString(SHA256Hash h) {
    return hexEncode((const char*)h.data(), h.size());
}

PublicWalletAddress walletAddressFromPublicKey(PublicKey inputKey) {
    // Based on: https://en.bitcoin.it/wiki/Technical_background_of_version_1_Bitcoin_addresses
    SHA256Hash hash;
    hash = SHA256((const char*)inputKey.data(), inputKey.size());
    RIPEMD160Hash hash2 = RIPEMD160((const char*)hash.data(), hash.size());
    SHA256Hash hash3 = SHA256((const char*)hash2.data(),hash2.size());
    SHA256Hash hash4 = SHA256((const char*)hash3.data(),hash3.size());
    uint8_t checksum = hash4[0];
    PublicWalletAddress address;
    address[0] = 0;
    for(int i = 1; i <=20; i++) {
        address[i] = hash2[i-1];
    }
    address[21] = hash4[0];
    address[22] = hash4[1];
    address[23] = hash4[2];
    address[24] = hash4[3];
    
    return address;

}

std::pair<PublicKey,PrivateKey> generateKeyPair() {
    char buf[32];
    ed25519_create_seed((unsigned char*)buf);
    PublicKey pubKey;
    PrivateKey privKey;
    ed25519_create_keypair((unsigned char*)pubKey.data(), (unsigned char*)privKey.data(), (unsigned char*)buf);
    return std::pair<PublicKey, PrivateKey>(pubKey, privKey);
}

string walletAddressToString(PublicWalletAddress p) {
    return hexEncode((const char*)p.data(), p.size());
}

PublicWalletAddress stringToWalletAddress(string s) {
    vector<uint8_t> bytes = hexDecode(s);
    PublicWalletAddress ret;
    std::move(bytes.begin(), bytes.begin() + ret.size(), ret.begin());
    return ret;
}

string privateKeyToString(PrivateKey p) {
    return hexEncode((const char*)p.data(), p.size());
}
PrivateKey stringToPrivateKey(string p) {
    vector<uint8_t> bytes = hexDecode(p);
    PrivateKey pKey;
    std::move(bytes.begin(), bytes.begin() + pKey.size(), pKey.begin());
    return pKey;
}

string publicKeyToString(PublicKey pubKey) {
    return hexEncode((const char*) pubKey.data(),pubKey.size());
}
PublicKey stringToPublicKey(string p) {
    vector<uint8_t> data = hexDecode(p);
    PublicKey pubKey;
    for(int i = 0; i < data.size(); i++) {
        pubKey[i] = data[i];
    }
    return pubKey;
}   

TransactionSignature signWithPrivateKey(string content, PublicKey pubKey, PrivateKey privKey) {
    return signWithPrivateKey(content.c_str(), content.length(), pubKey, privKey);
}

TransactionSignature signWithPrivateKey(const char* bytes, size_t len, PublicKey pubKey, PrivateKey privKey) {
    TransactionSignature t;
    ed25519_sign(
        (unsigned char *)t.data(), 
        (const unsigned char *) bytes, len, 
        (const unsigned char *) pubKey.data(), 
        (const unsigned char *) privKey.data()
    );
    return t;
}

string signatureToString(TransactionSignature t) {
    return hexEncode((const char*) t.data(), t.size());
}
TransactionSignature stringToSignature(string t) {
    vector<uint8_t> data = hexDecode(t);
    TransactionSignature decoded;
    for(int i = 0; i < data.size(); i++) {
        decoded[i] = data[i];
    }
    return decoded;
}

std::uint64_t getCurrentTime() {
    // HACK: return representable time
    return std::time(0);
}

std::string timeToString(const std::uint64_t& t) {
    std::ostringstream oss;
    oss << t;
    return oss.str();
}

std::uint64_t stringToTime(const std::string& input) {
    std::istringstream stream(input);
    uint64_t t;
    stream >> t;
    return t;
}