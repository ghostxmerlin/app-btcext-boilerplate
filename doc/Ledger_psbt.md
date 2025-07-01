# Ledger's stradgey of psbt sign

```slasing psbt```

<span style="color:red">70736274ff01007d0200000001</span>
<span style="color:red">4e683ee71f41038076b119c0938306fa731df680fe8ac45f1c1bc5e95b31ec60</span>
<span style="color:red">00000000</span>
<span style="color:red">ffffffff</span>
02
<span style="color:red">c409000000000000</span>
<span style="color:red">1600145be12624d08a2b424095d7c07221c33450d14bf1</span>
<span style="color:red">04a6000000000000</span>
<span style="color:red">2251202c95bad50a63d13aa818df8e4b6864181adbf4720a88aaf8e3c1235ba08a4d9f</span>
<span style="color:red">00000000</span>
01012b50c3000000000000
<span style="color:yellow">225120d763de6b471e305641ba41d65c6782e8cbcff6e08e83daab0da1275bbc9faad</span>
<span style="color:green">04215c150929b74c1a04954b78b4b6035e97a5e078a5a0f28ec96d547bfee9ace803ac089b605f98831c3e526d9eb2179651452938a8c0ff7f5eaeeccb61251d5d46debfd790120dc8d2f9eff0c4f4dbde070a48e330efc908b62a766568d91e658f284b324b878ad20d66124f8f42fd83e4c901a100ae3b5d706ef6cfd217b04bc64152e739a30c41ead200aee0509b16db71c999238a4827db945526859b13c95487ab46725357c9a9f25ac20113c3a32a9d320b72190a04a020a0db3976ef36972673258e9a38a364f3dc3b0ba2017921cf156ccb4e73d428f996ed11b245313e37e27c978ac4d2cc21eca4672e4ba203bb93dfc8b61887d771f3630e9a63e97cbafcfcc78556a474df83a31a0ef899cba2040afaf47c4ffa56de86410d8e47baa2bb6f04b604f4ea24323737ddc3fe092dfba2079a71ffd71c503ef2e2f91bccfc8fcda7946f4653cef0d9f3dde20795ef3b9f0ba20d21faf78c6751a0d38e6bd8028b907ff07e9a869a43fc837d6b3f8dff6119a36ba20f5199efae3f28bb82476163a7e458c7ad445d9bffb0682d10d3bdb2cb41f8e8eba20fa9d882d45f4060bdb8042183828cd87544f1ea997380e586cab77d5fd698737ba569cc00117</span>
<span style="color:yellow">2050929b74c1a04954b78b4b6035e97a5e078a5a0f28ec96d547bfee9ace803ac</span>
<span style="color:red">00000000</span>

<span style="color:red">basic info to make sighash</span></br>
<span style="color:yellow">info to verify input</span></br>
<span style="color:green">to compute leafhash for taproot script path spend</span></br>


## Do not transfer entire psbt to firmware -- secure reason

 Transfer data only necessary and keep it minimal, under the parsing of TS SDK
 ```
 basic info: version/locktime
 input info: prevout hash/index/nSequ
 output info: scriptPubkey/amount
 ```
For common transaction, data above is enough to make sighash.

If taproot scriptpath, the **sighash** need input script leafhash
```
Using policy(descriptor+HMAC)

descriptor:
tr(@0/**,and_v(pk_k(@1/**),and_v(pk_k(@2/**),multi_a(6,@3/**,@4/**,@5/**,@6/**,@7/**,@8/**,@9/**,@10/**,@11/**))))"

HMAC
if (!is_standard_wallet_policy(...) && !verify_hmac(...)) {
    SEND_SW(dc, SW_SECURITY_POLICY_REJECTED);
    return;
}

```

## 2 verify input
For common transaction:
```
psbt info: in witness_utxo: scriptPubKey
xpub(fingerprint check, full derivation path) --> pubkey === scriptPubKey
```
## 3 verify output 

For common transaction:
```
outputs' scriptPubkey --> Address + amount --> show to users
users get these infomation from 2nd channel(DM,mail,phone,f2f) to confirm
NO TRUST WHAT SOFTWARE UI SHOWS BUT

ledger's screen === 2nd channel

```




