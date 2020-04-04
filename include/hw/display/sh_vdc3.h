#include "qemu/osdep.h"
#include "hw/hw.h"
#include "hw/qdev.h"
#include "ui/console.h"
#include "hw/sh4/sh.h"
#include "qapi/error.h"

typedef struct {
    DeviceState parent_obj;
    MemoryRegion *sysmem;
    MemoryRegion iomem;
    MemoryRegion iomem_fffc;
    MemoryRegion *vram;
    MemoryRegionSection fbsection;
    uint16_t w;
    uint16_t h;
    uint32_t base;
    uint32_t vram_base;
    QemuConsole *con;
    bool invalidate;
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

#define TYPE_SH_VDC3 "sh_vdc3"
#define SH_VDC3(obj) OBJECT_CHECK(sh_vdc3_state, (obj), TYPE_SH_VDC3)
