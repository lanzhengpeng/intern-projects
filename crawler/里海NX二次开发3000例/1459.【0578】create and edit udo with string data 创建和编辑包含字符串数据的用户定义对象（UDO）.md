### 【0578】create and edit udo with string data 创建和编辑包含字符串数据的用户定义对象（UDO）

#### 代码

```cpp
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <uf.h>
#include <uf_ui.h>
#include <uf_udobj.h>
#include <uf_object_types.h>
#include <uf_obj.h>
#include <uf_exit.h>
#include <uf_disp.h>
#include <uf_part.h>
#include <stdarg.h>
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
static UF_UDOBJ_class_t TEXT_class_id = 0;
DllExport extern UF_UDOBJ_class_t get_text_class_id(void)
{
    return TEXT_class_id;
}
static void register_text_udo(void)
{
    UF_CALL(UF_UDOBJ_create_class("text_UDO", "text UDO", &TEXT_class_id));
}
/* 里海 */
void ufsta(char *param, int *retcode, int paramLen)
{
    if (UF_CALL(UF_initialize())) return;
    register_text_udo();
    UF_terminate();
}
static tag_t ask_next_udo(tag_t part, UF_UDOBJ_class_t class_id, tag_t udo)
{
    UF_UDOBJ_class_t
        this_class_id;
    UF_UDOBJ_all_data_t
        udo_data;
    while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part,
        UF_user_defined_object_type, &udo)) && (udo != NULL_TAG))
    {
        UF_CALL(UF_UDOBJ_ask_udo_data(udo, &udo_data));
        this_class_id = udo_data.class_id;
        UF_CALL(UF_UDOBJ_free_udo_data(&udo_data));
        if (this_class_id == class_id) return udo;
    }
    return NULL_TAG;
}
static void report_udo_string_data(tag_t udo)
{
    int
        ii;
    UF_UDOBJ_all_data_t
        data;
    UF_CALL(UF_UDOBJ_ask_udo_data(udo, &data));
    ECHO("UDO has %d strings\n", data.num_strings);
    for (ii = 0; ii < data.num_strings; ii++)
        ECHO("\t%d. %s\n", ii+1, data.strings[ii]);
    UF_CALL(UF_UDOBJ_free_udo_data(&data));
}
static void do_it(void)
{
    int
        ii,
        resp;
    tag_t
        part = UF_PART_ask_display_part(),
        udo = NULL_TAG;
    UF_UDOBJ_class_t
        text_class = get_text_class_id();
    char
        **editted_text,
        new_line[133],
        *new_text[1],
        *original_text[1] = { "Original text string" };
    UF_UDOBJ_all_data_t
        data;
    if (text_class == 0)
    {
        register_text_udo();
        text_class = get_text_class_id();
    }
    if ((udo = ask_next_udo(part, text_class, udo)) == NULL_TAG)
    {
        UF_CALL(UF_UDOBJ_create_udo(text_class, &udo));
        UF_CALL(UF_UDOBJ_add_strings(udo, 1, original_text));
    }
    else
    {
        UF_CALL(UF_UDOBJ_ask_udo_data(udo, &data));
        editted_text = (char **)UF_allocate_memory(
            data.num_strings * sizeof(char *), &resp);
        for (ii = 0; ii < data.num_strings - 1; ii++)
        {
            editted_text[ii] = (char *)UF_allocate_memory(
                strlen(data.strings[ii]) * sizeof(char), &resp);
            strcpy(editted_text[ii], data.strings[ii]);
        }
        editted_text[data.num_strings-1] = (char *)UF_allocate_memory(
            (strlen(data.strings[data.num_strings-1])+22)*sizeof(char), &resp);
        sprintf(editted_text[data.num_strings-1], "%s - editted at rev %d",
            data.strings[data.num_strings-1], data.num_strings);
        UF_CALL(UF_UDOBJ_edit_strings(udo, editted_text));
        UF_free_string_array(data.num_strings, editted_text);
        new_text[0] = new_line;
        sprintf(new_line, "text string added at rev %d", data.num_strings);
        UF_CALL(UF_UDOBJ_add_strings(udo, 1, new_text));
        UF_CALL(UF_UDOBJ_free_udo_data(&data));
    }
    report_udo_string_data(udo);
}
/* 里海 */
void ufusr(char *param, int *retcode, int paramLen)
{
    if (UF_CALL(UF_initialize())) return;
    do_it();
    UF_terminate();
}

```

#### 代码解析

> 这段代码是NX Open的一个示例，实现了创建和使用用户定义对象(UDO)的功能。主要功能包括：
>
> 1. 定义了一个名为"text_UDO"的UDO类，用于存储字符串数据。
> 2. 在NX部件中遍历所有UDO对象，找到"text_UDO"类的对象，如果没有则创建一个。
> 3. 每次运行时，将原有字符串数据加上版本号后缀，然后添加一条新的字符串数据。
> 4. 输出当前UDO对象包含的字符串数据。
> 5. 使用了错误报告宏，可以输出函数调用失败的信息。
> 6. 使用NX Open提供的API实现了UDO的创建、修改和查询等功能。
> 7. UDO类和对象的创建封装在函数中，以避免重复创建。
> 8. 输出信息到日志窗口和系统日志。
> 9. 使用NX Open的内存管理函数进行内存分配和释放。
> 10. 提供了ufusr和ufsta两个函数入口点，分别用于在NX启动和关闭时执行代码。
>
> 该代码实现了对NX部件中UDO对象的基本操作，为NX Open中的UDO开发提供了示例。
>
