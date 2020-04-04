#include "hw/display/sh_vdc3.h"

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
#define GRCMEN_WE(reg) ((reg >> 31) & 0x01)
#define GRCMEN_WE_PROHIBIT 0
#define GRCMEN_WE_PERMIT 1
#define GRCMEN_WE_MASK 0x80000000U

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
            if (GRCMEN_WE(s->grcmen2) == GRCMEN_WE_PERMIT) {
                s->w = (s->gropswh2 >> 0) & 0x03FF;
                s->h = (s->gropswh2 >> 16) & 0x03FF;
                qemu_console_resize(s->con, s->w, s->h);
                s->grcmen2 &= ~GRCMEN_WE_MASK;
            }
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

static void sh_vdc3_invalidate_display(void *opaque)
{
    sh_vdc3_state *s = SH_VDC3(opaque);

    s->invalidate = true;
}

static void sh_vdc3_update_display(void *opaque)
{
    sh_vdc3_state *s = SH_VDC3(opaque);

    s->invalidate = false;
}

static const GraphicHwOps sh_vdc3_gfx_ops = {
    .invalidate = sh_vdc3_invalidate_display,
    .gfx_update = sh_vdc3_update_display,
};

static void sh_vdc3_init(Object *obj)
{
    sh_vdc3_state *s = SH_VDC3(obj);

    memory_region_init_io(&s->iomem, NULL, &sh_vdc3_ops, s,
                          "vdc3", 0x100000000ULL);

    memory_region_init_alias(&s->iomem_fffc, NULL, "vdc3-fffc", &s->iomem,
                             0, VDC3_SIZE);
}

static void sh_vdc3_realize(DeviceState *dev, Error **errp)
{
    sh_vdc3_state *s = SH_VDC3(dev);
    Error *err = NULL;
    Object *obj;

    obj = object_property_get_link(OBJECT(dev), "sysmem", &err);
    if (obj == NULL) {
        abort();
    }
    s->sysmem = MEMORY_REGION(obj);
    memory_region_add_subregion(s->sysmem, s->base, &s->iomem_fffc);

    s->con = graphic_console_init(dev, 0, &sh_vdc3_gfx_ops, s);
}

static Property sh_vdc3_props[] = {
    DEFINE_PROP_UINT32("base", sh_vdc3_state, base, 0),
    DEFINE_PROP_END_OF_LIST()
};

static void sh_vdc3_class_init(Object *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);

    dc->props = sh_vdc3_props;
    dc->realize = sh_vdc3_realize;
}

static TypeInfo sh_vdc3_info = {
    .name = TYPE_SH_VDC3,
    .parent = TYPE_DEVICE,
    .instance_size = sizeof(sh_vdc3_state),
    .class_init = sh_vdc3_class_init,
    .instance_init = sh_vdc3_init,
};

static void sh_vdc3_register_types(void)
{
    type_register_static(&sh_vdc3_info);
}

type_init(sh_vdc3_register_types)
