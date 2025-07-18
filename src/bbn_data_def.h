// TLV标签定义
#define TAG_ACTION_TYPE         0x77
#define TAG_FP_COUNT           0xf9
#define TAG_FP_LIST            0xf8
#define TAG_COV_KEY_COUNT      0xc0
#define TAG_COV_KEY_LIST       0xc1
#define TAG_STAKER_PK          0x51
#define TAG_COV_QUORUM         0x01
#define TAG_STAKE_TIMELOCK     0x71
#define TAG_UNBONDING_TIMELOCK 0x72
#define TAG_SLASHING_FEE_LIMIT 0xfe
#define TAG_UNBONDING_FEE_LIMIT 0xff

// Action Type定义
#define ACTION_STAKING          1
#define ACTION_UNBOND           2
#define ACTION_SLASHING         3
#define ACTION_UNBONDING_SLASHING 4
#define ACTION_WITHDRAW         5
#define ACTION_SIGN_MESSAGE     6