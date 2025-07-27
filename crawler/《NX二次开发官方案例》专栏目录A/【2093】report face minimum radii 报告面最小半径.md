### 【2093】report face minimum radii 报告面最小半径

#### 代码

```cpp
/*HEAD REPORT_FACE_MINIMUM_RADII CCC UFUN */
#include <stdio.h>
#include <uf.h>
#include <uf_object_types.h>
#include <uf_part.h>
#include <uf_modl.h>
#include <uf_ui.h>
#include <uf_disp.h>
#include <uf_obj.h>
#define ECHO(X)    (printf("%s = %d\n", #X, X))
#define ECHOF(X)   (printf("%s = %f\n", #X, X))
#define ECHO2F(X)  (printf("%s = %f, %f\n",#X,*(X),*(X+1)))
#define ECHO3F(X)  (printf("%s = %f, %f, %f\n",#X,*(X),*(X+1),*(X+2)))
#define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))
static int report_error( char *file, int line, char *call, int irc)
{
    if (irc)
    {
        char err[133],
             msg[133];
        sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",
            irc, line, file);
        UF_get_fail_message(irc, err);
    /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：请注意，UF_print_syslog 是 V18 版本新增的功能，请只回答翻译，不要添加其他无关内容。 */
        UF_print_syslog(msg, FALSE);
        UF_print_syslog(err, FALSE);
        UF_print_syslog("\n", FALSE);
        UF_print_syslog(call, FALSE);
        UF_print_syslog(";\n", FALSE);
        if (!UF_UI_open_listing_window())
        {
            UF_UI_write_listing_window(msg);
            UF_UI_write_listing_window(err);
            UF_UI_write_listing_window("\n");
            UF_UI_write_listing_window(call);
            UF_UI_write_listing_window(";\n");
        }
    }
    return(irc);
}
/* qq3123197280 */
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
    UF_CALL(UF_UI_select_with_single_dialog(prompt, "",
        UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,
        &object, cp, &view));
    if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)
    {
        UF_CALL(UF_DISP_set_highlight(object, FALSE));
        return object;
    }
    else return NULL_TAG;
}
static void report_face_min_radii(tag_t face)
{
    int
        ii,
        num_rad,
        face_type;
    double
        min_rad[2],
        pos[6],
        params[4];
    char
        msg[MAX_LINE_SIZE+1];
    UF_CALL(UF_UI_open_listing_window());
    UF_CALL(UF_MODL_ask_face_type(face, &face_type));
    sprintf(msg, "Face Tag:  %d\n", face);
    UF_CALL(UF_UI_write_listing_window(msg));
    sprintf(msg, "Face Type: %d\n", face_type);
    UF_CALL(UF_UI_write_listing_window(msg));
    UF_CALL(UF_MODL_ask_face_min_radii(face, &num_rad, min_rad, pos, params));
    sprintf(msg, "Number of mins: %d\n-----------------------\n",num_rad);
    UF_CALL(UF_UI_write_listing_window(msg));
    for (ii = 0; ii < num_rad; ii++)
    {            
        sprintf(msg,"Min radius(%d):  %f\n",
            ii+1,min_rad[ii]);
        UF_CALL(UF_UI_write_listing_window(msg));
        sprintf(msg, "Position:       %f,%f,%f\n",
            pos[(ii*3)+0], pos[(ii*3)+1], pos[(ii*3)+2]);
        UF_CALL(UF_UI_write_listing_window(msg));
        sprintf(msg, "U-V parameters: %f, %f\n------------\n", 
            params[(ii*2)+0], params[(ii*2)+1]);
        UF_CALL(UF_UI_write_listing_window(msg));
    }
}
static void do_it(void)
{
    tag_t
        face;
    while ((face = select_a_face("Select a face")) != NULL_TAG)
    {
        report_face_min_radii(face);
    }
}
/* qq3123197280 */
void ufusr(char *param, int *retcode, int paramLen)
{
    if (!UF_CALL(UF_initialize()))
    {
        do_it();
        UF_CALL(UF_terminate());
    }
}
int ufusr_ask_unload(void)
{
    return (UF_UNLOAD_IMMEDIATELY);
}

```

#### 代码解析

> 这段代码是基于NX的二次开发代码，主要功能是报告选择面的最小半径。以下是代码的主要功能：
>
> 1. 定义宏：使用宏定义了错误报告函数report_error，用于在函数调用失败时打印错误信息。
> 2. 选择面：定义了mask_for_faces和select_a_face两个函数，用于通过对话框选择一个面。
> 3. 报告最小半径：定义了report_face_min_radii函数，用于查询选择面的最小半径并打印相关信息。
> 4. 主函数：do_it函数循环调用select_a_face选择面，然后调用report_face_min_radii报告每个面的最小半径。
> 5. NX初始化：ufusr函数用于初始化NX，调用do_it函数，然后终止NX。
> 6. 卸载请求：ufusr_ask_unload函数用于在卸载时立即终止NX。
>
> 总体来说，这段代码实现了在NX中查询选择面的最小半径并打印相关信息的功能，采用了NX提供的UF函数进行二次开发。
>
