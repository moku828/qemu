#include "qemu/osdep.h"
#include "hw/hw.h"
#include "hw/sh4/sh.h"
#include "qapi/error.h"

#define VDC3_SIZE 0x1928
#define GRCMEN2_OFS 0x1000
#define GRCBUSCNT2_OFS 0x1004
#define GROPSADR2_OFS 0x1308
#define GROPSWH2_OFS 0x130C
#define GROPSOFST2_OFS 0x1310
#define GROPDPHV2_OFS 0x1314
#define SGMODE_OFS 0x1800
#define SGINTCNT_OFS 0x1804
#define SYNCNT_OFS 0x1808
#define PANEL_CLKSEL_OFS 0x180C
#define SYN_SIZE_OFS 0x1900
#define PANEL_VSYNC_TIM_OFS 0x1904
#define PANEL_HSYNC_TIM_OFS 0x1908
#define GRA_VSYNC_TIM_OFS 0x1910

typedef struct {
    MemoryRegion iomem;
    MemoryRegion iomem_fffc;
    uint32_t grcmen2;
    uint32_t grcbuscnt2;
    uint32_t gropsadr2;
    uint32_t gropswh2;
    uint32_t gropsofst2;
    uint32_t gropdphv2;
    uint32_t sgmode;
    uint32_t sgintcnt;
    uint32_t syncnt;
    uint32_t panel_clksel;
    uint32_t syn_size;
    uint32_t panel_vsync_tim;
    uint32_t panel_hsync_tim;
    uint32_t gra_vsync_tim;
} sh_vdc3_state;

static uint64_t sh_vdc3_read(void *opaque, hwaddr offs,
                             unsigned size)
{
    sh_vdc3_state *s = opaque;

    if (size == 1) {
        switch (offs) {
        default:
            abort();
        }
    } else if (size == 2) {
        switch (offs) {
        default:
            abort();
        }
    } else if (size == 4) {
        switch (offs) {
        case GRCMEN2_OFS:
            return s->grcmen2;
        case GRCBUSCNT2_OFS:
            return s->grcbuscnt2;
        case GROPSADR2_OFS:
            return s->gropsadr2;
        case GROPSWH2_OFS:
            return s->gropswh2;
        case GROPSOFST2_OFS:
            return s->gropsofst2;
        case GROPDPHV2_OFS:
            return s->gropdphv2;
        case SGMODE_OFS:
            return s->sgmode;
        case SGINTCNT_OFS:
            return s->sgintcnt;
        case SYNCNT_OFS:
            return s->syncnt;
        case PANEL_CLKSEL_OFS:
            return s->panel_clksel;
        case SYN_SIZE_OFS:
            return s->syn_size;
        case PANEL_VSYNC_TIM_OFS:
            return s->panel_vsync_tim;
        case PANEL_HSYNC_TIM_OFS:
            return s->panel_hsync_tim;
        case GRA_VSYNC_TIM_OFS:
            return s->gra_vsync_tim;
        default:
            abort();
        }
    } else {
        abort();
    }

    return 0;
}

static void sh_vdc3_write(void *opaque, hwaddr offs,
                          uint64_t val, unsigned size)
{
    sh_vdc3_state *s = opaque;

    if (size == 1) {
        switch (offs) {
        default:
            abort();
        }
    } else if (size == 2) {
        switch (offs) {
        default:
            abort();
        }
    } else if (size == 4) {
        switch (offs) {
        case GRCMEN2_OFS:
            s->grcmen2 = val;
            break;
        case GRCBUSCNT2_OFS:
            s->grcbuscnt2 = val;
            break;
        case GROPSADR2_OFS:
            s->gropsadr2 = val;
            break;
        case GROPSWH2_OFS:
            s->gropswh2 = val;
            break;
        case GROPSOFST2_OFS:
            s->gropsofst2 = val;
            break;
        case GROPDPHV2_OFS:
            s->gropdphv2 = val;
            break;
        case SGMODE_OFS:
            s->sgmode = val;
            break;
        case SGINTCNT_OFS:
            s->sgintcnt = val;
            break;
        case SYNCNT_OFS:
            s->syncnt = val;
            break;
        case PANEL_CLKSEL_OFS:
            s->panel_clksel = val;
            break;
        case SYN_SIZE_OFS:
            s->syn_size = val;
            break;
        case PANEL_VSYNC_TIM_OFS:
            s->panel_vsync_tim = val;
            break;
        case PANEL_HSYNC_TIM_OFS:
            s->panel_hsync_tim = val;
            break;
        case GRA_VSYNC_TIM_OFS:
            s->gra_vsync_tim = val;
            break;
        default:
            abort();
        }
    } else {
        abort();
    }
}

static const MemoryRegionOps sh_vdc3_ops = {
    .read = sh_vdc3_read,
    .write = sh_vdc3_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
};

void sh_vdc3_init(MemoryRegion *sysmem,
                  hwaddr base)
{
    sh_vdc3_state *s;

    s = g_malloc0(sizeof(sh_vdc3_state));

    memory_region_init_io(&s->iomem, NULL, &sh_vdc3_ops, s,
                          "vdc3", 0x100000000ULL);

    memory_region_init_alias(&s->iomem_fffc, NULL, "vdc3-fffc", &s->iomem,
                             0, VDC3_SIZE);
    memory_region_add_subregion(sysmem, base, &s->iomem_fffc);
}
