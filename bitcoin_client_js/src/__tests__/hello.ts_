import fs from 'fs';

import SpeculosTransport from "@ledgerhq/hw-transport-node-speculos-http";
import Transport from '@ledgerhq/hw-transport-node-speculos-http';
import { AppClient, DefaultWalletPolicy, PsbtV2 } from ".."

describe('AppClient with Speculos', () => {
  let transport: Transport;
  let app: AppClient;

  beforeAll(async () => {
    transport = await Transport.open('http://127.0.0.1:5000' as any);
    app = new AppClient(transport);
  });

  afterAll(async () => {
    if (transport) await transport.close();
  });

it('should get master fingerprint', async () => {
  const fpr = await app.getMasterFingerprint();
  expect(typeof fpr).toBe('string');
  expect(fpr.length).toBe(8); // 4字节 Buffer 的 hex 表示是8位
});

it('should call custom XOR APDU', async () => {
  const CLA_APP = 0xe1; // 替换为你的 CLA
  const INS_CUSTOM_XOR = 128;
  const data = Buffer.from([1, 2, 3, 4]); // 1^2^3^4 = 4

  const response = await transport.send(CLA_APP, INS_CUSTOM_XOR, 0, 0, data);
  const result = response.slice(0, -2);
  const sw = response.readUInt16BE(response.length - 2);

  expect(sw).toBe(0x9000);
  expect(result[0]).toBe(1 ^ 2 ^ 3 ^ 4); // 4
});
// Sets the speculos automation file using the REST api.
// TODO: It would be better to add this in SpeculosTransport, or create a new custom class.
async function setSpeculosAutomation(transport: SpeculosTransport, automationObj: any): Promise<void> {
  return new Promise((resolve, reject) => {
      transport.instance
        .post(`/automation`, automationObj)
        .then((response) => {
          resolve(response.data);
        }, reject);
    });
}

it("can sign a psbt", async () => {
    // psbt from test_sign_psbt_singlesig_wpkh_2to2 in the main test suite, converted to PSBTv2
    const psbtBuf = Buffer.from(
      "cHNidP8BAAoBAAAAAAAAAAAAAQIEAgAAAAEDBAAAAAABBAECAQUBAgH7BAIAAAAAAQBxAgAAAAGTarLgEHL3k8/kyXdU3hth/gPn22U2yLLyHdC1dCxIRQEAAAAA/v///wLe4ccAAAAAABYAFOt418QL8QY7Dj/OKcNWW2ichVmrECcAAAAAAAAWABQjGNZvhP71xIdfkzsDjcY4MfjaE/mXHgABAR8QJwAAAAAAABYAFCMY1m+E/vXEh1+TOwONxjgx+NoTIgYDRV7nztyXsLpDW4AGb8ksljo0xgAxeYHRNTMMTuQ6x6MY9azC/VQAAIABAACAAAAAgAAAAAABAAAAAQ4gniz+J/Cth7eKI31ddAXUowZmyjYdWFpGew3+QiYrTbQBDwQBAAAAARAE/f///wESBAAAAAAAAQBxAQAAAAEORx706Sway1HvyGYPjT9pk26pybK/9y/5vIHFHvz0ZAEAAAAAAAAAAAJgrgoAAAAAABYAFDXG4N1tPISxa6iF3Kc6yGPQtZPsrwYyAAAAAAAWABTcKG4M0ua9N86+nsNJ+18IkFZy/AAAAAABAR9grgoAAAAAABYAFDXG4N1tPISxa6iF3Kc6yGPQtZPsIgYCcbW3ea2HCDhYd5e89vDHrsWr52pwnXJPSNLibPh08KAY9azC/VQAAIABAACAAAAAgAEAAAAAAAAAAQ4gr7+uBlkPdB/xr1m2rEYRJjNqTEqC21U99v76tzesM/MBDwQAAAAAARAE/f///wESBAAAAAAAIgICKexHcnEx7SWIogxG7amrt9qm9J/VC6/nC5xappYcTswY9azC/VQAAIABAACAAAAAgAEAAAAKAAAAAQMIqDoGAAAAAAABBBYAFOs4+puBKPgfJule2wxf+uqDaQ/kAAEDCOCTBAAAAAAAAQQiACA/qWbJ3c3C/ZbkpeG8dlufr2zos+tPEQSq1r33cyTlvgA=",
      "base64"
    );

    const automation = JSON.parse(fs.readFileSync('src/__tests__/automations/sign_with_wallet_accept.json').toString());
    await setSpeculosAutomation(transport, automation);

    const walletPolicy = new DefaultWalletPolicy(
      "wpkh(@0/**)",
      "[f5acc2fd/84'/1'/0']tpubDCtKfsNyRhULjZ9XMS4VKKtVcPdVDi8MKUbcSD9MJDyjRu1A2ND5MiipozyyspBT9bg8upEp7a8EAgFxNxXn1d7QkdbL52Ty5jiSLcxPt1P"
    );

    const psbt = new PsbtV2();
    psbt.deserialize(psbtBuf);
    const result = await app.signPsbt(psbt, walletPolicy, null, () => {});

    expect(result.length).toEqual(2);

    expect(result[0][0]).toEqual(0);
    expect(result[0][1].pubkey).toEqual(Buffer.from(
      "03455ee7cedc97b0ba435b80066fc92c963a34c600317981d135330c4ee43ac7a3",
      "hex"
    ));
    expect(result[0][1].signature).toEqual(Buffer.from(
      "304402206b3e877655f08c6e7b1b74d6d893a82cdf799f68a5ae7cecae63a71b0339e5ce022019b94aa3fb6635956e109f3d89c996b1bfbbaf3c619134b5a302badfaf52180e01",
      "hex"
    ));


    expect(result[1][0]).toEqual(1);
    expect(result[1][1].pubkey).toEqual(Buffer.from(
      "0271b5b779ad870838587797bcf6f0c7aec5abe76a709d724f48d2e26cf874f0a0",
      "hex"
    ));
    expect(result[1][1].signature).toEqual(Buffer.from(
      "3045022100e2e98e4f8c70274f10145c89a5d86e216d0376bdf9f42f829e4315ea67d79d210220743589fd4f55e540540a976a5af58acd610fa5e188a5096dfe7d36baf3afb94001",
      "hex"
    ));
    expect(result[1][1].tapleafHash).toBeUndefined();
  });
});