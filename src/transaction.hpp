#pragma once
#include "crypto.hpp"
using namespace std;

struct TransactionInfo {
    char signature[64];
    char signingKey[32];
    uint64_t timestamp;
    PublicWalletAddress to;
    PublicWalletAddress from;
    TransactionAmount amount;
    TransactionAmount fee;
    bool isTransactionFee;
};

class Transaction {
    public:
        Transaction(json t);
        Transaction();
        Transaction(const Transaction & t);
        Transaction(PublicWalletAddress to, TransactionAmount fee);
        Transaction(PublicWalletAddress from, PublicWalletAddress to, TransactionAmount amount, PublicKey signingKey, TransactionAmount fee=0);
        Transaction(PublicWalletAddress from, PublicWalletAddress to, TransactionAmount amount, PublicKey signingKey, TransactionAmount fee, uint64_t timestamp);
        Transaction(const TransactionInfo& t);
        TransactionInfo serialize();
        json toJson();
        void sign(PublicKey pubKey, PrivateKey signingKey);
        void setTransactionFee(TransactionAmount amount);
        TransactionAmount getTransactionFee() const;
        void setAmount(TransactionAmount amt);
        PublicWalletAddress fromWallet() const;
        PublicWalletAddress toWallet() const;
        TransactionAmount getAmount() const;
        void setTimestamp(uint64_t t);
        uint64_t getTimestamp();
        SHA256Hash getHash() const;
        SHA256Hash hashContents() const;
        TransactionSignature getSignature() const;
        bool isFee() const;
    protected:
        TransactionSignature signature;
        PublicKey signingKey;
        uint64_t timestamp;
        PublicWalletAddress to;
        PublicWalletAddress from;
        TransactionAmount amount;
        TransactionAmount fee;
        bool isTransactionFee;
        friend bool operator==(const Transaction& a, const Transaction& b);
        friend bool operator<(const Transaction& a, const Transaction& b);
};

bool operator==(const Transaction& a, const Transaction& b);

