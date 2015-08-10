#define NUM_TRIM_ELEMS 6
#define NUM_BOX_ELEMS 8
#define NUM_BAR_ELEMS 8
#define NUM_TOGGLE_ELEMS 4

#ifndef _MODEL_H_
#define _MODEL_H_


//This cannot be computed, and must be manually updated
#define NUM_PROTO_OPTS 4
#define VIRT_NAME_LEN 10


struct Model {
    u8 num_channels;
    s16 proto_opts[NUM_PROTO_OPTS];
    u32 fixed_id;
    enum TxPower tx_power;
};

extern struct Model Model;

#endif /*_MODEL_H_*/
