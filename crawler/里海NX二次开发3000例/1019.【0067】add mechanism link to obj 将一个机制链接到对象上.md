### 【0067】add mechanism link to obj 将一个机制链接到对象上

#### 代码

```cpp
    /*HEAD ADD_MECHANISM_LINK_TO_OBJ CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_mech.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：

UF_print_syslog是V18版本新增的功能。 */  
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
    static int mask_for_bodies(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_body(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bodies, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static tag_t ask_next_mechanism(tag_t mechanism)  
    {  
        int  
            eighty_five = 85,  
            irc;  
        FTN(uf6010)(&mechanism, &eighty_five, &irc);  
        if (irc == 0) return mechanism;  
        else return NULL_TAG;  
    }  
    static void report_object_name(char *name, tag_t object)  
    {  
        char  
            obj_name[UF_OBJ_NAME_LEN + 1];  
        printf("%s - tag %d is ", name, object);  
        if (!UF_OBJ_ask_name(object, obj_name)) printf("named %s\n", obj_name);  
        else printf("not named\n");  
    }  
    static void do_it(void)  
    {  
        int  
            irc;  
        tag_t  
            mechanism = NULL_TAG,  
            body = NULL_TAG,  
            nr4;  
        while ((mechanism = ask_next_mechanism(mechanism)) != NULL_TAG)  
        {  
            report_object_name("mechanism", mechanism);  
            FTN(uf6002)(&mechanism, &irc);  
            if (irc == 0)  
                while (( body = select_a_body("Select body"))!= NULL_TAG)  
                {  
                    UF_CALL(uc6031("myLink001", 1, &body, &nr4));  
                }  
            else  
                 printf("irc:  \n", irc);  
        }  
    }  
    /* 里海 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        UF_set_retiring_flag(2 ); /*notify about retiring functions */  
            do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段NX二次开发代码的主要功能是实现选择一个机构（mechanism）并为其添加一个链接对象（link object）。具体步骤如下：
>
> 1. 初始化UF函数库，设置退役函数标志。
> 2. 不断循环获取当前打开的机构，直到没有更多机构为止。
> 3. 输出当前机构的名称和tag。
> 4. 获取当前机构下的所有部件（bodies），并循环选择一个部件。
> 5. 为选中的部件创建一个名为"myLink001"的链接对象，并将其添加到当前机构下。
> 6. 重复步骤2-5，直到没有更多机构为止。
> 7. 释放UF函数库资源。
> 8. 实现了ufusr_ask_unload函数，以便在用户请求卸载时立即卸载。
>
> 该代码实现了选择机构和为机构添加链接对象的功能，同时提供了错误处理和提示信息，以便用户友好地使用。
>
