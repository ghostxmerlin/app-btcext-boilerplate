#include <stdbool.h>
#include <stdint.h>
#include "bbn_data_def.h"
#include "display.h"


bool parse_tlv_data(const uint8_t *data, uint32_t data_len) {
    uint32_t offset = 0;
    
    PRINTF("=== TLV Data Parsing ===\n");
    
    while (offset < data_len) {
        if (offset + 1 >= data_len) {
            PRINTF("Error: Not enough data for TAG\n");
            return false;
        }
        
        uint8_t tag = data[offset++];
        
        // 读取长度（假设长度字段为1-2字节）
        uint16_t length = 0;
        if (offset >= data_len) {
            PRINTF("Error: Not enough data for LENGTH\n");
            return false;
        }
        
        if (offset + 2 > data_len) {
            PRINTF("Error: Not enough data for 2-byte LENGTH\n");
            return false;
        }
        length = (data[offset] << 8) | data[offset + 1];
        offset += 2;
        
        if (offset + length > data_len) {
            PRINTF("Error: Not enough data for VALUE (need %d bytes, have %d)\n", 
                   length, (int)(data_len - offset));
            return false;
        }
        
        const uint8_t *value = &data[offset];
        
        PRINTF("TAG: 0x%02x, LEN: %d, VALUE: ", tag, length);
        PRINTF_BUF(value, length);
        
        // 根据TAG类型解析具体内容
        switch (tag) {
            case TAG_ACTION_TYPE:
                PRINTF("  -> Action Type\n");
                if (length >= 1 && value != NULL) {
                    const char* action_names[] = {
                        "UNKNOWN", "STAKING", "UNBOND", "SLASHING", 
                        "UNBONDING_SLASHING", "WITHDRAW", "SIGN_MESSAGE"
                    };
                    uint8_t action = value[0];
                    PRINTF("  -> Action Type %d\n", action);
                } else {
                    PRINTF("  -> Invalid Action Type length or NULL value\n");
                }
                break;
            case TAG_FP_COUNT:
                if (length == 1) {
                    PRINTF("  -> Finality Provider Count: %d\n", value[0]);
                } else {
                    PRINTF("  -> Invalid FP Count length\n");
                }
                break;
                
            case TAG_FP_LIST:
                PRINTF("  -> Finality Provider List (%d pubkeys):\n", length / 32);
                for (int i = 0; i < length / 32; i++) {
                    PRINTF("    FP[%d]: ", i);
                    PRINTF_BUF(value + i * 32, 32);
                }
                break;
                
            case TAG_COV_KEY_COUNT:
                if (length == 1) {
                    PRINTF("  -> Covenant Key Count: %d\n", value[0]);
                } else {
                    PRINTF("  -> Invalid Cov Key Count length\n");
                }
                break;
                
            case TAG_COV_KEY_LIST:
                PRINTF("  -> Covenant Key List (%d pubkeys):\n", length / 32);
                for (int i = 0; i < length / 32; i++) {
                    PRINTF("    COV[%d]: ", i);
                    PRINTF_BUF(value + i * 32, 32);
                }
                break;
                
            case TAG_STAKER_PK:
                if (length == 32) {
                    PRINTF("  -> Staker Public Key: ");
                    PRINTF_BUF(value, 32);
                } else {
                    PRINTF("  -> Invalid Staker PK length\n");
                }
                break;
                
            case TAG_COV_QUORUM:
                if (length == 1) {
                    PRINTF("  -> Covenant Quorum: %d\n", value[0]);
                } else {
                    PRINTF("  -> Invalid Cov Quorum length\n");
                }
                break;
                
            case TAG_STAKE_TIMELOCK:
                if (length == 8) {
                    uint64_t timelock = read_u64_le(value, 0);
                    PRINTF("  -> Stake Timelock: %d\n", timelock);
                } else {
                    PRINTF("  -> Invalid Stake Timelock length\n");
                }
                break;
                
            case TAG_UNBONDING_TIMELOCK:
                if (length == 8) {
                    uint64_t timelock = read_u64_le(value, 0);
                    PRINTF("  -> Unbonding Timelock: %lld\n", timelock);
                } else {
                    PRINTF("  -> Invalid Unbonding Timelock length\n");
                }
                break;
                
            case TAG_SLASHING_FEE_LIMIT:
                if (length == 8) {
                    uint64_t limit = read_u64_le(value, 0);
                    PRINTF("  -> Slashing Fee Limit: %lld satoshi\n", limit);
                } else {
                    PRINTF("  -> Invalid Slashing Fee Limit length\n");
                }
                break;
                
            case TAG_UNBONDING_FEE_LIMIT:
                if (length == 8) {
                    uint64_t limit = read_u64_le(value, 0);
                    PRINTF("  -> Unbonding Fee Limit: %lld satoshi\n", limit);
                } else {
                    PRINTF("  -> Invalid Unbonding Fee Limit length\n");
                }
                break;
                
            default:
                PRINTF("  -> Unknown TAG: 0x%02x\n", tag);
                break;
        }
        
        offset += length;
    }
    
    PRINTF("=== TLV Parsing Complete ===\n");
    return true;
}