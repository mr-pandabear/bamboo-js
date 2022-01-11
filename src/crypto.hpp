#pragma once
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#include "json.hpp"
using namespace nlohmann;
using namespace std;


typedef std::array<uint8_t, 25> PublicWalletAddress;
typedef uint64_t TransactionAmount;
typedef std::array<uint8_t,32> PublicKey;
typedef std::array<uint8_t,64> PrivateKey;
typedef std::array<uint8_t,64> TransactionSignature;
typedef std::array<uint8_t, 32> SHA256Hash;
typedef std::array<uint8_t, 20> RIPEMD160Hash;

std::uint64_t getCurrentTime();
std::string timeToString(const std::uint64_t& t);
std::uint64_t stringToTime(const std::string& input);

SHA256Hash SHA256(const char* buffer, size_t len);
SHA256Hash SHA256(string str);
RIPEMD160Hash  RIPEMD(const char* buffer, size_t len);
SHA256Hash stringToSHA256(string hex);
string SHA256toString(SHA256Hash h);
std::vector<uint8_t> hexDecode(const string& hex);
string hexEncode(const char* buffer, size_t len);

PublicWalletAddress walletAddressFromPublicKey(PublicKey inputKey);
string walletAddressToString(PublicWalletAddress p);
PublicWalletAddress stringToWalletAddress(string s);
std::pair<PublicKey,PrivateKey> generateKeyPair();
string publicKeyToString(PublicKey p);
PublicKey stringToPublicKey(string p);
string privateKeyToString(PrivateKey p);
PrivateKey stringToPrivateKey(string p);

string signatureToString(TransactionSignature t);
TransactionSignature stringToSignature(string t);

TransactionSignature signWithPrivateKey(string content, PublicKey pubKey, PrivateKey privKey);
TransactionSignature signWithPrivateKey(const char* bytes, size_t len, PublicKey pubKey, PrivateKey privKey);