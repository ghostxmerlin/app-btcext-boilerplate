# Plan for new framework

## What Ledger offers

1.custom_apdu_handler:  
with this handler, we could make our USB command easily.

2.validate_and_display_transaction(custom):     
make our own way to validate and display

3.sign_custom_inputs(custom).    
Make sighash and sign


## Transfer data with custom apdu(Data Preparing)
1.To transfer data for Babylon actions by a new custom apdu.   
2.Do not user descriptor in policy but user our own data struct to handle device

## Use taproot defualt policy
By using dafualt policy, we keep sign psbt function simple and only transfer the basic info of transaction(version, sequ, txid, output). We add leafhash which computerd by data from Data Preparing after validating.

## Validate input

If we want to check the input address is from wallet address and correct for the current staking status, we need not only check the staker pk and leafhash of the script, but comupte the merkle root and tweak to the address and compare it to output.

```
Action              input                               data need to compute address
------------------------------------------------------------------------------------
slashing            stake                               staker,fp,cov,stake time
unbond slashing     unbond                              staker,cov,unbond time
unbond              stake                               staker,fp,cov,stake time
withdraw            unbond/slashing/unbond slasing      all above

```

The main purpose to validate input is to avoid sign transactions not belong to wallet which is considered as a factor that increase the risk of private key leakage. However, whether this risk justifies the additional resources, increased data size, and more complex display interactions required for mitigation is worth careful consideration. Based on current experience, many hardware wallets do not implement such protections, and even auditors rarely raise concerns about this issue.

```
lv   how                     
------------------------------------------------------------------------------------
1    no validation           minimizes data transmission and display
2    leafhash+staker pk      first version implement
3    address                 ? Implement in new framework
```

## Validate output

The first version implement we validate the output address === tweak result with data taproot script merkle root hash. It ensures that the address received by Ledger is exactly consistent with the information provided by the frontend, without any tampering. 

However, it does not guarantee that the user’s transaction remains secure if the frontend code has been maliciously altered—yet this is precisely the most critical security property of a hardware wallet: even if the PC software is compromised, the user should still be able to verify the transaction details on the Ledger screen to ensure the safety of their coins.

In a common transaction, users typically obtain the output address through a method that is independent of the frontend and confirm it by checking the device screen. Although this process is simple, it is highly effective in enabling users to detect whether the transaction data they are about to sign has been tampered with.

Unfortunately, in Babylon’s staking transactions, this kind of verification becomes significantly more difficult. The destination address is not a plain public key or account, but rather a cryptographic hash derived from one or more scripts. As a result, users have no practical way to obtain or independently reconstruct the output address.

While we do display key components used in the address computation and the final resulting address, the information is often lengthy, abstract, and hard to interpret—making meaningful verification challenging for most users.

At its core, the problem stems from the fact that all of this data is delivered to Ledger by the frontend. If the frontend is compromised, the displayed information can also be manipulated accordingly, and Ledger will still show values that match what the compromised frontend provides. This offers no real guarantee of user safety.

Even though it may appear that we’ve fulfilled our duty by showing the data, the true purpose of a hardware wallet is not to present information—it is to ensure the absolute security of user coins.

## Tomo's proposal

Therefore, Tomo proposes developing a standalone calculator specifically for computing staking addresses. Ideally, this tool should run on a separate platform—such as a mobile device—independent of the frontend.

The user would input the public key retrieved from Ledger and repeat the same selection steps they performed in the frontend. The calculator would then compute and display the final derived address. This allows the user to cross-check the result with the address shown on the Ledger device screen.

In fact, under this model, the user only needs to verify the final output address. All other display elements become merely auxiliary and, if necessary, can even be omitted entirely.

## How does the caculator work

1. Users download APP to their phones.
2. For Flex and Stax, ledger app shows staker pk in QR code and APP scan it to get. For nano, qr code could be shown on tomo's website for scan and user could verify it by nano's screen.
3. Users go the simple-staking like websites, choose fp, amount and timelock to start staking.
4. Meanwhile, open the APP and do the same operations and get the staking/slashing/unbond slashing/unbond address
5. Users confirm these addresses on Ledger's screen before approving the transaction.


