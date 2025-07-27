### 【1513】list assembly objects 列表装配对象

#### 代码

```cpp
    /*HEAD LIST_ASSEMBLY_OBJECTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog是V18版本新增的功能，请只回答翻译内容，不要添加其他无关内容。 */  
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
    static void cycle_component(tag_t component, char *indentation)  
    {  
        int  
            ii = 0,  
            subtype,  
            type;  
        tag_t  
            member = NULL_TAG;  
        char  
            indent[MAX_LINE_SIZE+1],  
            messg[MAX_LINE_SIZE+1];  
        while ((!UF_CALL(UF_ASSEM_cycle_objs_in_comp(component, &member))) &&  
                (member != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(member, &type, &subtype));  
            sprintf(messg, "%s%d.  %d - %d/%d\n",  
                indentation, ++ii, member, type, subtype);  
            UF_CALL(UF_UI_write_listing_window(messg));  
            if (type == UF_component_type)  
            {  
                strcpy(indent, indentation);  
                strcat(indent, "    ");  
                cycle_component(member, indent);  
            }  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            root_part_occ = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        if ((part != NULL_TAG) &&  
            (root_part_occ = UF_ASSEM_ask_root_part_occ(part)) != NULL_TAG)  
        {  
            UF_CALL(UF_UI_open_listing_window());  
            cycle_component(root_part_occ, "");  
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

> 这段代码是用于NX的二次开发，其主要功能是遍历装配结构并打印出每个组件的信息。
>
> 代码的主要功能包括：
>
> 1. 初始化NX环境。
> 2. 获取当前显示的零件，并获取其根组件。
> 3. 递归遍历每个组件，打印出组件的tag、类型和子类型信息。
> 4. 如果组件是另一个组件，则继续递归遍历。
> 5. 打印信息到NX的列表窗口中。
> 6. 释放NX环境。
>
> 主要函数和流程如下：
>
> 1. do_it函数：获取当前显示的零件，并获取其根组件。然后调用cycle_component函数递归遍历组件。
> 2. cycle_component函数：遍历组件内的对象，如果对象是组件则递归调用自身。打印每个对象的tag、类型和子类型信息。
> 3. ufusr函数：二次开发入口函数，初始化NX环境，调用do_it函数，然后释放NX环境。
> 4. report_error函数：用于打印错误信息。
> 5. ufusr_ask_unload函数：用于卸载二次开发模块。
>
> 总体来说，这段代码实现了对NX装配结构的遍历和打印，是一个典型的NX二次开发示例。
>
