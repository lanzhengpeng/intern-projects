### 【3056】wave link all solids 将装配体中的所有实体体进行波浪链接

#### 代码

```cpp
/*HEAD WAVE_LINK_ALL_SOLIDS CCC UFUN */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <uf.h>
#include <uf_ui.h>
#include <uf_object_types.h>
#include <uf_disp.h>
#include <uf_wave.h>
#include <uf_so.h>
#include <uf_assem.h>
#include <uf_obj.h>
#include <uf_part.h>
#include <uf_modl.h>
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
    /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，V18版本新增了UF_print_syslog函数，用于打印系统日志。以下是该函数的简单介绍：

UF_print_syslog() 是一个新函数，在V18版本中引入。它用于打印系统日志。

函数原型如下：

void UF_print_syslog(const char *fmt, ...);

其中fmt是格式化字符串，后面可以跟任意个参数。该函数会将这些参数按照fmt指定的格式，打印到系统日志中。

使用示例：

UF_print_syslog("Login failed for user %s", username);

该示例会打印一条登录失败的系统日志，其中%s会被替换为变量username的值。

综上所述，UF_print_syslog()是一个新引入的函数，用于向系统日志打印格式化的消息。 */
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
static tag_t ask_next_body(tag_t part, tag_t body)
{
    int
        subtype,
        type;
    while (! UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body))
        && (body != NULL_TAG))
    {
        UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));
        if (subtype == UF_solid_body_subtype) return body;
    }
    return NULL_TAG;
}
static int allocate_memory(unsigned int nbytes, void **where)
{
    int
        resp;
    *where = UF_allocate_memory(nbytes, &resp);
    return resp;
}
static int make_an_array(uf_list_p_t *object_list, tag_t **objects)
{
    int
        ii,
        n;
    uf_list_p_t
        temp;
    UF_CALL(UF_MODL_ask_list_count(*object_list, &n));
    UF_CALL(allocate_memory(n * sizeof(tag_t), (void **)objects));
    for (ii = 0, temp = *object_list; ii < n; temp = temp->next, ii++)
        (*objects)[ii] = temp->eid;
    UF_CALL(UF_MODL_delete_list(object_list));
    return n;
}
static int ask_all_bodies(tag_t part, tag_t **solids)
{
    tag_t
        body = NULL_TAG;
    uf_list_p_t
        body_list;
    UF_CALL(UF_MODL_create_list(&body_list));
    while ((body = ask_next_body(part, body)) != NULL_TAG)
        UF_CALL(UF_MODL_put_list_item(body_list, body));
    return (make_an_array(&body_list, solids));
}
static tag_t ask_part_occ_of_prom_occ(tag_t prom_object)
{
    tag_t
        owning_part,
        part_occ = prom_object,
        proto;
    proto = UF_ASSEM_ask_prototype_of_occ(prom_object);
    UF_CALL(UF_OBJ_ask_owning_part(proto, &owning_part));
    while (!UF_CALL(UF_ASSEM_ask_parent_component(part_occ, &part_occ))
        && (UF_ASSEM_ask_prototype_of_occ(part_occ) != owning_part));
    return part_occ;
}
static void create_wave_linked_body(tag_t body)
{
    tag_t
        feat,
        part_occ,
        proto,
        xform;
    if (UF_ASSEM_is_occurrence(body))
        proto = UF_ASSEM_ask_prototype_of_occ(body);
    else
        proto = body;
    if (UF_OBJ_is_object_a_promotion(proto))
        part_occ = ask_part_occ_of_prom_occ(body);
    else
        part_occ = UF_ASSEM_ask_part_occurrence(body);
    if (part_occ == NULL_TAG)
        UF_CALL(UF_WAVE_create_linked_body(proto,NULL_TAG,body,FALSE,&feat));
    else
    {
        UF_CALL(UF_SO_create_xform_assy_ctxt(body, part_occ, NULL_TAG, &xform));
        UF_CALL(UF_WAVE_create_linked_body(proto, xform, body, FALSE, &feat));
    }
    UF_CALL(UF_MODL_update());
}
static void do_it(void)
{
    int
        ii,
        n_bodies;
    tag_t
        *bodies,
        part = UF_PART_ask_display_part();
    n_bodies = ask_all_bodies(part, &bodies);
    for (ii = 0; ii < n_bodies; ii++)
        create_wave_linked_body(bodies[ii]);
    if (n_bodies > 0) UF_free(bodies);
}
/* qq3123197280 */
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

> 这段代码是一个NX的二次开发示例，主要功能是将当前显示部件中的所有实体创建为波形连接体。
>
> 主要步骤包括：
>
> 1. 包含必要的NX API头文件。
> 2. 定义错误报告函数，用于打印函数调用失败的信息。
> 3. 定义一个辅助函数，用于遍历部件中的所有实体，并返回下一个实体的tag。
> 4. 定义内存分配函数。
> 5. 定义一个函数，用于将列表转换为数组。
> 6. 定义一个函数，用于获取部件中的所有实体，并返回实体数组和数量。
> 7. 定义一个函数，用于获取推广实体的部件实例。
> 8. 定义一个函数，用于创建波形连接体。
> 9. 定义主函数，用于获取当前显示部件中的所有实体，并遍历创建波形连接体。
> 10. 定义ufusr函数，作为程序的入口。
> 11. 定义卸载函数。
>
> 该代码通过遍历部件中的所有实体，并为每个实体创建波形连接体，实现了波形连接所有实体的功能。同时，代码中包含了错误处理和内存管理，是一个结构清晰、功能完整的二次开发示例。
>
