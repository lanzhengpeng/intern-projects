### 【0072】add part attribute columns to part family template 添加部件属性列到部件族模板

#### 代码

```cpp
/*HEAD ADD_PART_ATTRIBUTE_COLUMNS_TO_PART_FAMILY_TEMPLATE CCC UFUN */
#include <stdio.h>
#include <string.h>
#include <uf.h>
#include <uf_ui.h>
#include <uf_part.h>
#include <uf_fam.h>
#include <uf_attr.h>
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
    /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，UF_print_syslog是在V18版本中新增的。 */
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
static logical prompt_for_text(char *prompt, char *text)
{
    int
        n,
        resp;
    resp = uc1600(prompt, text, &n);
    if (resp == 3 || resp == 5) return TRUE;
    return FALSE;
}
static void do_it(void)
{
    logical
        is_template;
    int
        err,
        family_count;
    tag_t
        *families,
        part = UF_PART_ask_display_part();
    char
        attr_title[UF_ATTR_MAX_TITLE_LEN] = { "" },
        attr_string[UF_ATTR_MAX_STRING_LEN] = { "" };
    UF_ATTR_value_t
        attr_value;
    UF_FAM_attribute_data_t
        attribute_data;
    UF_FAM_family_data_t
        family_data;
    UF_CALL(UF_PART_is_family_template(part, &is_template));
    if (!is_template) return;
    UF_CALL(UF_PART_ask_families(part, &family_count, &families));
    UF_CALL(UF_FAM_ask_family_data(families[0], &family_data));
    attribute_data.subtype = UF_fam_attr_part_subtype;
    UF_CALL(UF_ATTR_ask_part_attribute(&attribute_data.base_object));
    attribute_data.rules = "";
    attribute_data.value = attr_string;
    attribute_data.name = attr_title;
    while (prompt_for_text("Enter part attribute title", attr_title) &&
           prompt_for_text("Default attribute value", attr_string))
    {
        attr_value.type = UF_ATTR_string;
        attr_value.value.string = attr_string;
        UF_CALL(UF_ATTR_assign(part, attr_title, attr_value));
        family_data.attributes = UF_reallocate_memory(family_data.attributes,
            (family_data.attribute_count+1) * sizeof(tag_t), &err);
        UF_CALL(UF_FAM_create_attribute(&attribute_data,
            &family_data.attributes[family_data.attribute_count]));
        family_data.attribute_count++;
        UF_CALL(UF_FAM_edit_family(families[0], &family_data));
    }
    UF_CALL(UF_FAM_free_family_data(&family_data));
    UF_free(families);
}
/* 里海 */
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

> 这段代码是一个NX的二次开发代码，主要功能是在零件族模板上添加零件属性。
>
> 主要功能包括：
>
> 1. 检查当前打开的零件是否为零件族模板，如果不是则退出。
> 2. 获取当前零件的零件族信息。
> 3. 交互式提示用户输入零件属性的标题和默认值，然后为当前零件添加该属性。
> 4. 重复步骤3，直到用户不再输入。
> 5. 释放内存，结束。
>
> 代码中使用了NX提供的UF_PART、UF_FAM、UF_ATTR等模块的函数，实现了零件属性的管理。另外，代码还包含了一个错误报告函数report_error，用于在函数调用失败时打印错误信息。整体来说，代码结构清晰，功能明确，符合NX二次开发的一般规范。
>
