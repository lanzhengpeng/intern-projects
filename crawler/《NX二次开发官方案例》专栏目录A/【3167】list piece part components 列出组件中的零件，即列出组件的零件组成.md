### 【3167】list piece part components 列出组件中的零件，即列出组件的零件组成

#### 代码

```cpp
    /*HEAD LIST_PIECE_PART_COMPONENTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的函数。 */  
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
    static tag_t ask_next_component(tag_t part, tag_t comp)  
    {  
        int  
            subtype,  
            type;  
        while  (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_component_type, &comp))  
            && (comp != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(comp, &type, &subtype));  
            if (subtype == UF_part_occurrence_subtype) return comp;  
        }  
        return comp;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            comp = NULL_TAG,  
            comp_part,  
            part = UF_ASSEM_ask_work_part();  
        char  
            part_name[MAX_FSPEC_SIZE+1];  
        while ((comp = ask_next_component(part, comp)) != NULL_TAG)  
        {  
            comp_part = UF_ASSEM_ask_prototype_of_occ(comp);  
            if ((UF_ASSEM_ask_root_part_occ(comp_part)) == NULL_TAG)  
            {  
                UF_PART_ask_part_name(comp_part, part_name);  
                printf("comp %d - occ of %s is a piece part\n", comp, part_name);  
            }  
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

> 这段代码是一个NX的二次开发示例，主要功能是遍历装配体中的零件，并识别出那些作为组件的零件。
>
> 主要步骤如下：
>
> 1. 定义了错误报告函数report_error，用于在调用UF函数失败时报告错误。
> 2. 定义了ask_next_component函数，用于遍历装配体中的组件，并返回下一个组件的tag。
> 3. 定义了do_it函数，遍历装配体中的组件，获取组件对应的零件，然后判断该零件是否为根零件，如果不是，则输出该零件的名称，表明它是一个组件。
> 4. 定义了ufusr函数，这是NX二次开发的入口函数。初始化NX系统，调用do_it函数执行功能，然后终止NX系统。
> 5. 定义了ufusr_ask_unload函数，用于卸载用户二次开发应用程序。
>
> 通过该代码，可以学习如何遍历装配体中的组件，获取组件对应的零件，并判断零件是否为根零件。这对于进行装配体的组件分析非常有用。
>
