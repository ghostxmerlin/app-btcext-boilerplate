import Transport from '@ledgerhq/hw-transport-node-speculos-http';
import { AppClient, sendMerkleizedData } from '../lib/appClient';

describe('AppClient with Speculos', () => {
  let transport: any;
  let app: AppClient;

  beforeAll(async () => {
    transport = await Transport.open('http://127.0.0.1:5000' as any);
    app = new AppClient(transport);
  });

  afterAll(async () => {
    if (transport) await transport.close();
  });

  it('send merkleized 1024 bytes', async () => {
    const buffer = Buffer.alloc(1024, 0xab);
    const response = await sendMerkleizedData(app, 0xbb, buffer);

    const sw = response.readUInt16BE(response.length - 2);
    expect(sw).toBe(0x9000);
  });
});