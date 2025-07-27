### 【0872】create scenario idealize feature 创建理想化特征场景

#### 代码

```cpp
    /*HEAD CREATE_SCENARIO_IDEALIZE_FEATURE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_sf.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog 是在 V18 版本中新增的。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
    static void do_it(void)  
    {  
        int  
            n_failing_wound_edges;  
        tag_t  
            body,  
            exp,  
            feature,  
            *failing_wound_edges,  
            part = UF_PART_ask_display_part();  
        UF_SF_idealize_parms_p_t  
            parms_p;  
        if (UF_SF_is_scenario_part()) /* illogical TRUE = NOT 译:illogical TRUE = NOT */  
        {  
            uc1601("Work part not a scenario part", TRUE);  
            return;  
        }  
        if ((body = ask_next_body(part, NULL_TAG)) != NULL_TAG)  
        {  
            UF_CALL(UF_SF_alloc_idealize_parms(&parms_p));  
            UF_CALL(UF_MODL_create_exp_tag("2.0", &exp));  
            UF_CALL(UF_SF_set_idealize_parm_exp(parms_p,  
                UF_SF_IDEALIZE_HOLE_DIAMETER, exp));  
            if (!UF_CALL(UF_SF_create_idealize_body(body, parms_p, &feature,  
                &n_failing_wound_edges, &failing_wound_edges)))  
            {  
                WRITE_D(feature);  
                if (n_failing_wound_edges > 0) UF_free(failing_wound_edges);  
            }  
            UF_CALL(UF_SF_free_idealize_parms(parms_p));  
        }  
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

> 这段代码是NX Open的二次开发代码，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于打印错误信息。
> 2. 定义了一个整数写入列表窗口的函数write_integer_to_listing_window。
> 3. 定义了一个获取下一个实体的函数ask_next_body。
> 4. 主函数do_it中，获取当前显示的部件，遍历部件中的实体，并判断实体是否为实体体。如果是，则创建理想化参数结构，设置理想化直径参数，然后调用UF_SF_create_idealize_body函数对实体进行理想化处理，并打印出理想化特征和失败的边缘。
> 5. ufusr函数是二次开发的入口函数，初始化后调用do_it函数，最后终止。
> 6. ufusr_ask_unload函数用于卸载二次开发代码。
>
> 总的来说，这段代码实现了在NX中对实体进行理想化处理的二次开发功能。
>
