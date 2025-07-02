# What we did in current version

## NO change on TS SDK

There is no change of Ledger's bitcoin ts sdk. All difference is in firmware.

When calling the signpsbt(), TS SDK transfers data as the descriptor in policy as a common taproot transaction.

Note that, the function of descriptor is no longer for computing the leafhash but just to make SDK to send all necessary data to device. 

The firmware will compute the leafhash our own code and compare it to the valure comupted by TS SDK.

## Verify input

Since the input scriptPubkey is tweaked except staking transaction, the original input check function will fail for slashing/unbond/withdraw. We check the staker pk in script and leafhash of the input taproot script to make the input is related wallet.

##  Computed Output Address Matches Frontend, But Cannot Be Independently Verified

Since we collect all necessary data from each action, we could compute the slaing/unbond/stake address by tweaking the publick key with merkle root and make sure it === output scriptPubkey in psbt.

However, it only ensures that the output address in the PSBT matches the one derived from the Taproot script provided by the frontend via the TypeScript SDK. Unlike traditional transactions, users are not able to verify this address using information from an independent source(2nd channel).

The core issue is that during this staking process, users cannot cross-check the destination address shown on their Ledger device through any alternative method—unless they manually perform complex cryptographic computations themselves. 
