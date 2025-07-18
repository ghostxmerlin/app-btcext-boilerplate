import Transport from '@ledgerhq/hw-transport-node-speculos-http';
import { AppClient, sendMerkleizedData } from '../lib/appClient';
/*
TAG=1 LEN=2 Value
Finality provider count:      TAG 0xf9  LEN 00 0n      VALUE count
Finality provider list:       TAG 0xf8  LEN 32*n       VALUE n pubkey
Cov key count:                TAG 0xc0  LEN 00 0n      VALUE count
Cov key list:                 TAG 0xc1  LEN 32*n       VALUE n pubkey
staker pk:                    TAG 0x51  LEN 32         VALUE pubkey
cov quorum:                   TAG 0x01  LEN 00 01      VALUE quorum
stake timelock:               TAG 0x71  LEN 00 08      VALUE timelock uint64
unbonding timelock:           TAG 0x72  LEN 00 08      VALUE timelock uint64
slashing fee limit:           TAG 0xfe  LEN 00 08      VALUE limit uint64
unbonding fee limit:          TAG 0xff  LEN 00 08      VALUE limit uint64
*/

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