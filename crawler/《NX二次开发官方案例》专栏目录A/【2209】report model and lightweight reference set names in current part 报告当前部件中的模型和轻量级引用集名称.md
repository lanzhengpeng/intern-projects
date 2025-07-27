### 【2209】report model and lightweight reference set names in current part 报告当前部件中的模型和轻量级引用集名称

#### 代码

```cpp
    /*HEAD REPORT_MODEL_AND_LIGHTWEIGHT_REFERENCE_SET_NAMES_IN_CURRENT_PART CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    static tag_t ask_model_ref_set(tag_t part)  
    {  
        int  
            subtype,  
            type;  
        tag_t  
            refset = NULL_TAG;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_reference_set_type,  
            &refset)) && (refset != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(refset, &type, &subtype));  
            if (subtype == UF_reference_design_model_subtype) return refset;  
        }  
        return NULL_TAG;  
    }  
    static tag_t ask_lightweight_ref_set(tag_t part)  
    {  
        int  
            subtype,  
            type;  
        tag_t  
            refset = NULL_TAG;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_reference_set_type,  
            &refset)) && (refset != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(refset, &type, &subtype));  
            if (subtype == UF_reference_design_lightweight_subtype) return refset;  
        }  
        return NULL_TAG;  
    }  
    #define SHOW_NAME(X) report_object_name(#X, X)  
    static void report_object_name(char *name, tag_t object)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN],  
            obj_name[MAX_ENTITY_NAME_SIZE + 1];  
        if (!UF_OBJ_ask_name(object, obj_name))  
            sprintf(msg, "%s (tag %d) is named %s\n", name, object, obj_name);  
        else  
            sprintf(msg, "%s (tag %d) is not named\n", name, object);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            lightweight_refset,  
            model_refset,  
            part = UF_PART_ask_display_part();  
        model_refset = ask_model_ref_set(part);  
        SHOW_NAME(model_refset);  
        lightweight_refset = ask_lightweight_ref_set(part);  
        SHOW_NAME(lightweight_refset);  
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

> 这段NX二次开发代码的主要功能是获取当前活动部件中的模型参考集和轻量级参考集，并输出它们的名称和标识符。
>
> 主要流程如下：
>
> 1. 包含必要的NX Open头文件，定义宏用于错误报告和对象名称输出。
> 2. 定义了一个错误报告函数report_error，用于输出函数调用失败的信息。
> 3. 定义了两个函数ask_model_ref_set和ask_lightweight_ref_set，用于在部件中遍历查找模型参考集和轻量级参考集，并返回它们的标识符。
> 4. 定义了一个函数report_object_name，用于输出对象的名称。
> 5. 在do_it函数中，获取当前活动部件，然后调用上述函数找到模型参考集和轻量级参考集，并输出它们的名称。
> 6. 在ufusr函数中，初始化NX Open，调用do_it函数，然后终止NX Open。
> 7. 定义了一个卸载函数ufusr_ask_unload，用于在卸载NX Open时立即清理。
>
> 综上所述，这段代码通过NX Open API遍历当前部件中的参考集，并输出模型参考集和轻量级参考集的名称和标识符，以便进行二次开发中的引用管理。
>
