### 【0545】copy point csys2csys 实现坐标点从参考坐标系到目标坐标系的复制功能

#### 代码

```cpp
/*HEAD COPY_POINT_CSYS2CSYS CCC UFUN */
#include <stdio.h>
#include <string.h>
#include <uf.h>
#include <uf_ui.h>
#include <uf_object_types.h>
#include <uf_curve.h>
#include <uf_csys.h>
#include <uf_trns.h>
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
   /*  NOTE:  UF_print_syslog is new in V18 译:根据原文，UF_print_syslog 是 V18 新增的功能，要求只翻译不添加其他内容。 */
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
static int mask_for_csys(UF_UI_selection_p_t select, void *type)
{
   UF_UI_mask_t
       mask = { UF_coordinate_system_type, 0, 0 };
   if (!UF_CALL(UF_UI_set_sel_mask(select,
           UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))
       return (UF_UI_SEL_SUCCESS);
   else
       return (UF_UI_SEL_FAILURE);
}
static tag_t select_a_csys(char *prompt)
{
   int
       resp;
   double
       cp[3];
   tag_t
       object,
       view;
   UF_CALL(UF_UI_select_with_single_dialog(prompt, "",
       UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_csys, NULL, &resp,
       &object, cp, &view));
   if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)
   {
       UF_CALL(UF_DISP_set_highlight(object, 0));
       return object;
   }
   else return NULL_TAG;
}
static int mask_for_points(UF_UI_selection_p_t select, void *type)
{
   UF_UI_mask_t
       mask = { UF_point_type, 0, 0 };
   if (!UF_CALL(UF_UI_set_sel_mask(select,
           UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))
       return (UF_UI_SEL_SUCCESS);
   else
       return (UF_UI_SEL_FAILURE);
}
static tag_t select_a_point(char *prompt)
{
   int
       resp;
   double
       cp[3];
   tag_t
       object,
       view;
   UF_CALL(UF_UI_select_with_single_dialog(prompt, "",
       UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_points, NULL, &resp,
       &object, cp, &view));
   if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)
   {
       UF_CALL(UF_DISP_set_highlight(object, 0));
       return object;
   }
   else return NULL_TAG;
}
static void map_csys2csys(tag_t ref_csys, tag_t dest_csys, double vec[3])
{
   double
       d_csys[12],
       mx[12],
       r_csys[12];
   int
       irc;
   tag_t
       csys_mx;
   UF_CALL(UF_CSYS_ask_csys_info(ref_csys, &csys_mx, r_csys));
   UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, &r_csys[3]));
   UF_CALL(UF_CSYS_ask_csys_info(dest_csys, &csys_mx, d_csys));
   UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, &d_csys[3]));
   FTN(uf5940)(r_csys, d_csys, mx, &irc);
   FTN(uf5941)(vec, mx);
}
static void do_it(void)
{
   tag_t
       d_csys,
       point,
       r_csys;
   double
       pt[3];
   while ((r_csys = select_a_csys("Reference csys")) &&
          (d_csys = select_a_csys("Destination csys")) &&
          (point = select_a_point("Point to copy")))
   {
       UF_CALL(UF_CURVE_ask_point_data(point, pt));
       map_csys2csys(d_csys, r_csys, pt);
       UF_CALL(UF_CURVE_create_point(pt, &point));
   }
}
void ufusr(char *param, int *retcode, int paramLen)
{
   if (UF_CALL(UF_initialize())) return;
   do_it();
   UF_terminate();
}
int ufusr_ask_unload(void)
{
   return (UF_UNLOAD_IMMEDIATELY);
}

```

#### 代码解析

> 这段代码实现了NX中两个坐标系之间的点复制功能。主要功能包括：
>
> 1. 错误报告函数：report_error，用于在函数调用失败时打印错误信息。
> 2. 选择坐标系和点的函数：select_a_csys和select_a_point，用于选择参考坐标系、目标坐标系和要复制的点。
> 3. 坐标系变换函数：map_csys2csys，用于将点从一个坐标系变换到另一个坐标系。
> 4. 主要执行函数：do_it，用于循环选择坐标系和点，并进行坐标系变换和点复制。
> 5. 用户函数入口：ufusr，用于初始化NX API并调用do_it函数。
> 6. 卸载函数：ufusr_ask_unload，用于设置模块的卸载方式。
>
> 代码流程为：用户先选择参考坐标系和目标坐标系，然后选择一个点。程序将这个点从参考坐标系变换到目标坐标系，并创建一个新的点。用户可以继续选择，直到取消选择为止。整体上，这段代码实现了NX中坐标系之间点的复制功能。
>
