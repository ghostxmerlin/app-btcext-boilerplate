import { AppClient } from '../index';
import Transport from '@ledgerhq/hw-transport-node-speculos-http';

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
});