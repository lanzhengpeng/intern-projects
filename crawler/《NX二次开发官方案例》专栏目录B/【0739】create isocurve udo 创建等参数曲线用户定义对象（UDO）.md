### 【0739】create isocurve udo 创建等参数曲线用户定义对象（UDO）

#### 代码

```cpp
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <uf.h>
#include <uf_ui.h>
#include <uf_udobj.h>
#include <uf_object_types.h>
#include <uf_obj.h>
#include <uf_exit.h>
#include <uf_disp.h>
#include <uf_part.h>
#include <uf_modl.h>
#include <uf_so.h>
static void ECHO(char *format, ...)
{
    char msg[UF_UI_MAX_STRING_LEN+1];
    va_list args;
    va_start(args, format);
    vsprintf(msg, format, args);
    va_end(args);
    UF_UI_open_listing_window();
    UF_UI_write_listing_window(msg);
    UF_print_syslog(msg, FALSE);
}
#define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))
static int report_error( char *file, int line, char *call, int irc)
{
    if (irc)
    {
        char err[133];
        UF_get_fail_message(irc, err);
        ECHO("*** ERROR code %d at line %d in %s:\n",
            irc, line, file);
        ECHO("+++ %s\n", err);
        ECHO("%s;\n", call);
    }
    return(irc);
}
static UF_UDOBJ_class_t ISO_class_id = 0;
DllExport extern UF_UDOBJ_class_t get_ISO_class_id(void)
{
    return ISO_class_id;
}
/* 里海 */
DllExport extern void isocurve_update(tag_t udo,
    UF_UDOBJ_link_p_t update_cause)
{
    int
        dir,
        ii,
        n_curves,
        subtype,
        status,
        type;
    tag_t
        *curves,
        face,
        exp;
    double
        fparm,
        parm,
        tol,
        uvs[4];
    UF_UDOBJ_all_data_t
        data;
    if (UF_CALL(UF_initialize())) return;
    UF_CALL(UF_UDOBJ_ask_owned_objects(udo, &n_curves, &curves));
    for (ii = 0; ii < n_curves; ii++)
        UF_CALL(UF_UDOBJ_delete_owning_link(udo, curves[ii]));
    if (n_curves > 0)
    {
        UF_CALL(UF_OBJ_delete_array_of_objects(n_curves, curves, NULL));
        UF_free(curves);
    }
    UF_CALL(UF_UDOBJ_ask_udo_data( udo, &data ));
    dir = data.ints[0];
    tol = data.doubles[0];
    for (ii = 0; ii < data.num_links; ii++)
    {
        UF_CALL(UF_OBJ_ask_type_and_subtype(data.link_defs[ii].assoc_ug_tag,
            &type, &subtype));
        if (type == UF_scalar_type)
        {
            UF_CALL(UF_SO_ask_exp_of_scalar(data.link_defs[ii].assoc_ug_tag,
                &exp));
            UF_CALL(UF_MODL_ask_exp_tag_value(exp, &parm));
            break;
        }
    }
    for (ii = 0; ii < data.num_links; ii++)
    {
        UF_CALL(UF_OBJ_ask_type_and_subtype(data.link_defs[ii].assoc_ug_tag,
            &type, &subtype));
        if ((type == UF_solid_type) && (subtype == UF_solid_face_subtype))
        {
            face = data.link_defs[ii].assoc_ug_tag;
            status = UF_OBJ_ask_status(face);
            if (status != UF_OBJ_ALIVE) continue;
            UF_CALL(UF_MODL_ask_face_uv_minmax(face, uvs));
            fparm = parm * (uvs[2*dir-1] - uvs[2*dir-2]) + uvs[2*dir-2];
            if (!UF_CALL(UF_MODL_create_isocurve(face, dir, fparm, tol,
                    &curves, &n_curves)))
            {
                UF_CALL(UF_UDOBJ_add_owning_links(udo, n_curves, curves));
                UF_free(curves);
            }
        }
    }
    UF_CALL(UF_UDOBJ_free_udo_data( &data ));
    UF_terminate();
}
static void register_isocurve_udo(void)
{
    UF_CALL(UF_UDOBJ_create_class("isocurve_UDO", "ISOCURVE", &ISO_class_id));
    UF_CALL(UF_UDOBJ_register_update_cb(ISO_class_id, isocurve_update));
}
/* 里海 */
static int mask_for_faces(UF_UI_selection_p_t select, void *type)
{
    UF_UI_mask_t
        mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE };
    if (!UF_CALL(UF_UI_set_sel_mask(select,
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))
        return (UF_UI_SEL_SUCCESS);
    else
        return (UF_UI_SEL_FAILURE);
}
static tag_t select_a_face(char *prompt)
{
    int
        resp;
    double
        cp[3];
    tag_t
        object,
        view;
    UF_CALL(UF_UI_select_with_single_dialog("Select a face", prompt,
        UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,
        &object, cp, &view));
    if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)
    {
        UF_CALL(UF_DISP_set_highlight(object, FALSE));
        return object;
    }
    else return NULL_TAG;
}
static logical prompt_for_a_number(char *prompt, char *item, double *number)
{
    int
        irc,
        resp;
    char
        menu[1][16];
    double
        da[1];
    strcpy(&menu[0][0], item);
    da[0] = *number;
    resp = uc1609(prompt, menu, 1, da, &irc);
    if (resp == 3 || resp == 4)
    {
        *number = da[0];
        return TRUE;
    }
    else return FALSE;
}
static void strip_trailing_zeros(char *s)
{
    int ii;
    if (strchr(s, '.'))
    {
        for (ii = strlen(s) - 1; s[ii] == '0'; ii--) s[ii] = '\0';
        if (s[ii] == '.') s[ii] = '\0';
    }
}
static void display_colored_conehead(double origin[3], double vec[3], int color)
{
    UF_DISP_conehead_attrb_s
        attrb;
    UF_DISP_get_conehead_attrb(&attrb);
    attrb.color = color;
    UF_DISP_set_conehead_attrb(&attrb);
    UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, origin, vec, 0);
    UF_DISP_reset_conehead_attrb();
}
static logical show_face_parm_dirs(tag_t face)
{
    double
        junk[3],
        origin[3],
        parm[2],
        uvs[4],
        u_dir[3],
        v_dir[3];
    UF_CALL(UF_MODL_ask_face_uv_minmax(face, uvs));
    parm[0] = uvs[0];
    parm[1] = uvs[2];
    UF_CALL(UF_MODL_ask_face_props(face, parm, origin, u_dir, v_dir,
        junk, junk, junk, junk));
    display_colored_conehead(origin, u_dir, UF_OBJ_BLUE);
    display_colored_conehead(origin, v_dir, UF_OBJ_GREEN);
    return TRUE;
}
static logical pick_one_of_two_choices(char *prompt, char *option_one,
    char *option_two, int *choice)
{
    int
        resp;
    char
        options[2][38];
    strncpy(&options[0][0], option_one, 37);
    strncpy(&options[1][0], option_two, 37);
    options[0][37] = '\0';
    options[1][37] = '\0';
    resp = uc1603(prompt, *choice, options, 2);
    if ((resp > 4) && (resp < 19))
    {
        *choice = resp - 4;  /* returns 1 or 2 译:翻译为：返回1或2 */
        return TRUE;
    }
    else return FALSE;
}
static void do_it(void)
{
    int
        n_curves,
        dir = { 1 };
    tag_t
        face,
        exp,
        *isocurves,
        so_exp,
        udo;
    double
        fparm,
        parm = 0.5,
        tol,
        uvs[4];
    char
        *pprompts[2] = { "U Parm", "V Parm" },
        exp_str[UF_MAX_EXP_LENGTH];
    UF_UDOBJ_class_t
        iso_class = get_ISO_class_id();
    UF_UDOBJ_link_t
        link_defs[2];
    if (iso_class == 0)
    {
        register_isocurve_udo();
        iso_class = get_ISO_class_id();
    }
    UF_MODL_ask_distance_tolerance(&tol);
    while (((face = select_a_face("Select face for isocurve")) != NULL_TAG) &&
            show_face_parm_dirs(face) &&
            pick_one_of_two_choices("Which direction?", "U (Green)",
                "V (Blue)", &dir) &&
            prompt_for_a_number("Constant value (0 - 1)", pprompts[dir-1],
                &parm))
    {
        sprintf(exp_str, "%f", parm);
        strip_trailing_zeros(exp_str);
        UF_CALL(UF_MODL_create_exp_tag(exp_str, &exp));
        UF_CALL(UF_UDOBJ_create_udo(iso_class, &udo));
        UF_CALL(UF_SO_create_scalar_exp(exp, UF_SO_update_within_modeling,
            exp, &so_exp));
        link_defs[0].link_type = 1;
        link_defs[0].assoc_ug_tag = so_exp;
        link_defs[0].object_status = 0;
        link_defs[1].link_type = 1;
        link_defs[1].assoc_ug_tag = face;
        link_defs[1].object_status = 0;
        UF_CALL(UF_UDOBJ_add_links(udo, 2, link_defs));
        UF_CALL(UF_UDOBJ_add_integers(udo, 1, &dir));
        UF_CALL(UF_UDOBJ_add_doubles(udo, 1, &tol));
        UF_CALL(UF_MODL_ask_face_uv_minmax(face, uvs));
        fparm = parm * (uvs[2*dir-1] - uvs[2*dir-2]) + uvs[2*dir-2];
        if (!UF_CALL(UF_MODL_create_isocurve(face, dir, fparm, tol,
            &isocurves, &n_curves)) && (n_curves > 0))
        {
            UF_CALL(UF_UDOBJ_add_owning_links(udo, n_curves, isocurves));
            UF_free(isocurves);
        }
    }
}
/* 里海 */
void ufusr(char *param, int *retcode, int paramLen)
{
    if (UF_CALL(UF_initialize())) return;
    do_it();
    UF_terminate();
}
/* 里海 */
void ufsta(char *param, int *retcode, int paramLen)
{
    if (UF_CALL(UF_initialize())) return;
    register_isocurve_udo();
    UF_terminate();
}

```

#### 代码解析

> 这段NX二次开发代码主要实现了以下功能：
>
> 1. 定义了Echo函数：用于输出信息到日志窗口和系统日志。
> 2. 定义了UF_CALL宏：用于调用NX API函数，并在出错时输出错误信息。
> 3. 实现了isocurve_update函数：该函数根据用户定义对象(UDO)中的参数，在NX模型面上创建等参数线。
> 4. 实现了register_isocurve_udo函数：用于注册一个名为"ISOCURVE"的UDO类，并注册isocurve_update作为其更新回调函数。
> 5. 实现了mask_for_faces函数：用于设置选择器，仅允许选择面特征。
> 6. 实现了select_a_face函数：用于通过对话框让用户选择一个面。
> 7. 实现了prompt_for_a_number函数：用于提示用户输入一个数值。
> 8. 实现了strip_trailing_zeros函数：用于去除字符串尾部的零。
> 9. 实现了display_colored_conehead函数：用于在面上显示不同颜色的圆锥头，用于指示U向和V向。
> 10. 实现了show_face_parm_dirs函数：用于显示面的U向和V向。
> 11. 实现了pick_one_of_two_choices函数：用于让用户在两个选项中选择一个。
> 12. 实现了do_it函数：该函数是主函数，用于选择面、方向、参数值，创建UDO对象，并调用isocurve_update在面上创建等参数线。
> 13. 实现了ufusr函数：NX启动时调用的函数，调用do_it实现功能。
> 14. 实现了ufsta函数：NX启动时调用的函数，用于注册UDO类。
>
> 整体而言，这段代码通过NX提供的API，实现了交互式创建等参数线的功能，并且使用了NX的UDO机制来存储用户输入的参数。
>
