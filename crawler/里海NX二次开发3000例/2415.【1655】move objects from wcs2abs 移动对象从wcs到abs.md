### 【1655】move objects from wcs2abs 移动对象从wcs到abs

#### 代码

```cpp
/*HEAD MOVE_OBJECTS_FROM_WCS2ABS CCC UFUN */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <uf.h>
#include <uf_ui.h>
#include <uf_object_types.h>
#include <uf_csys.h>
#include <uf_trns.h>
#include <uf_mtx.h>
#define ECHO(X)    (printf("%s = %d\n", #X, X))
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
    /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 新增的函数。 */
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
static int select_objects(char *prompt, tag_t **objects)
{
    int
        cnt,
        ii,
        resp;
    tag_t
        *objs;
    UF_CALL(UF_UI_select_with_class_dialog( prompt, "",
        UF_UI_SEL_SCOPE_WORK_PART, NULL, NULL, &resp, &cnt, objects));
    objs = *objects;
    for (ii = 0; ii < cnt; ii++)
        UF_CALL(UF_DISP_set_highlight(objs[ii], 0));
    return cnt;
}
static void matrix_csys2csys(tag_t ref_csys, tag_t dest_csys, double mx[12])
{
    double
        csys[9],
        orig[3],
        tx1[12],
        tx2[12],
        tx3[12],
        tx4[12],
        v[3];
    int
        ii;
    tag_t
        csys_mx;
    UF_CALL(UF_CSYS_ask_csys_info(ref_csys, &csys_mx, orig));
    UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, csys));
    tx1[0] = csys[0];
    tx1[1] = csys[1];
    tx1[2] = csys[2];
    tx1[3] = 0;
    tx1[4] = csys[3];
    tx1[5] = csys[4];
    tx1[6] = csys[5];
    tx1[7] = 0;
    tx1[8] = csys[6];
    tx1[9] = csys[7];
    tx1[10] = csys[8];
    tx1[11] = 0;
    /* set up to translate from reference csys back to absolute 里海译:根据参考坐标系转换回绝对坐标系，请只回答翻译结果，不要提供其他信息。 */
    for (ii=0; ii<3; ii++) v[ii] = -orig[ii];
    FTN(uf5943)(v, tx2);
    /* combine this with the rotation matrix from the reference csys 里海译:结合这个旋转矩阵和参考坐标系中的旋转矩阵。 */
    FTN(uf5942)(tx2, tx1, tx3);
    UF_CALL(UF_CSYS_ask_csys_info(dest_csys, &csys_mx, orig));
    UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, csys));
    /* Invert the rotation from the destination matrix 里海译:翻译：反转目标矩阵的旋转 */
    tx2[0] = csys[0];
    tx2[1] = csys[3];
    tx2[2] = csys[6];
    tx2[3] = 0;
    tx2[4] = csys[1];
    tx2[5] = csys[4];
    tx2[6] = csys[7];
    tx2[7] = 0;
    tx2[8] = csys[2];
    tx2[9] = csys[5];
    tx2[10] = csys[8];
    tx2[11] = 0;
    /* set up to translate from abs to the destination csys 里海译:根据文档内容，翻译命令为："设置从ABS到目标坐标系的转换。" */
    FTN(uf5943)(orig, tx1);
    /* combine this with the inverted rotation csys above 里海译:将此与上述倒置旋转坐标系结合 */
    FTN(uf5942)(tx2, tx1, tx4);
    /* and the one from the reference csys 里海译:对于翻译“and the one from the reference csys”，请提供以下内容：

参考坐标系中的那一个 */
    FTN(uf5942)(tx3, tx4, mx);
}
static void do_it(void)
{
    int
        irc,
        n,
        one = 1,
        two = 2,
        zero = 0;
    tag_t
        abs_mx,
        abs_id,
        *datum,
        wcs_id,
        grp;
    double
        matrix[9],
        zeropt[3] = {0,0,0},
        mx[12];
    UF_MTX3_identity( matrix );
    UF_CALL(UF_CSYS_ask_wcs(&wcs_id));
    UF_CALL(UF_CSYS_create_matrix(matrix, &abs_mx));
    UF_CALL(UF_CSYS_create_temp_csys(zeropt, abs_mx, &abs_id));
    while ((n = select_objects("Select objects to reposition", &datum)) > 0)
    {
        matrix_csys2csys(wcs_id, abs_id, mx);
        FTN(uf5947)(mx, datum, &n, &one, &zero, &two, datum, &grp, &irc);
        UF_CALL(irc);
        UF_free(datum);
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

> 这段代码是NX Open C++ API编写的二次开发代码，其功能是从工作坐标系(WCS)转换选定的对象到绝对坐标系(ABS)。
>
> 主要功能包括：
>
> 1. 错误报告：定义了一个错误报告函数report_error，用于在函数调用失败时输出错误信息。
> 2. 选择对象：定义了一个选择对象的函数select_objects，用于让用户选择要转换的对象。
> 3. 坐标系转换：定义了一个坐标系转换函数matrix_csys2csys，用于计算两个坐标系之间的转换矩阵。
> 4. 执行转换：在do_it函数中，首先获取当前WCS的ID，然后创建一个绝对坐标系的临时坐标系。在循环中，让用户选择对象，计算WCS到ABS的转换矩阵，并调用UF_MODL_transform功能将这些对象转换到ABS坐标系。
> 5. 程序入口：ufusr函数是程序的入口，初始化NX Open，调用do_it执行转换，最后终止NX Open。
> 6. 卸载函数：定义了一个卸载函数ufusr_ask_unload，返回立即卸载。
>
> 总的来说，这段代码实现了NX工作坐标系到绝对坐标系的转换功能，用户可以通过选择对象进行转换。
>
