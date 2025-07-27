### 【0444】box an assembly 组件包围盒

#### 代码

```cpp
    /*HEAD BOX_AN_ASSEMBLY CCC UFUN */  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：

注意：UF_print_syslog 是在 V18 版本中新加入的，请只回答翻译，不要添加其他无关内容。 */  
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
    static tag_t ask_a_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        do  
        {  
            UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body));  
            if (body) UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
        } while (body && subtype);  
        return (body);  
    }  
    static void mk_box(double *arr)  
    {  
        char  
            *len[3],  
            len_t[3][20];  
        tag_t  
            box;  
        len[0] = &len_t[0][0];  
        len[1] = &len_t[1][0];  
        len[2] = &len_t[2][0];  
        sprintf(len_t[0], "%f", arr[3] - arr[0]);  
        sprintf(len_t[1], "%f", arr[4] - arr[1]);  
        sprintf(len_t[2], "%f", arr[5] - arr[2]);  
        UF_CALL(UF_MODL_create_block1(UF_NULLSIGN, arr, len, &box));  
        UF_CALL(UF_MODL_ask_feat_body(box, &box));  
        UF_CALL(UF_OBJ_set_color(box, UF_OBJ_YELLOW));  
    }  
    static void do_it(void)  
    {  
        int  
            i;  
        double  
            box[6],  
            minmax[6] = { 10000, 10000, 10000, -10000, -10000, -10000 };  
        tag_t  
            body = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        while( (body = ask_a_body(part, body)) != 0)  
        {  
            FTN(uf6540)(&body, &i, box);  
            if (!i)  
            {  
                for (i=0; i<3; i++) if (box[i] < minmax[i]) minmax[i] = box[i];  
                for (i=3; i<6; i++) if (box[i] > minmax[i]) minmax[i] = box[i];  
            }  
        }  
        mk_box(minmax);  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 根据代码，这是一段用于NX的二次开发代码，主要功能是遍历当前部件中的所有实体，找出最小和最大的边界，然后在这个边界内创建一个黄色立方体。具体来说：
>
> 1. 定义了宏UF_CALL，用于调用UF函数，并在出错时打印错误信息。
> 2. 定义了函数report_error，用于在出错时打印错误信息。
> 3. 定义了函数ask_a_body，用于遍历当前部件中的所有实体。
> 4. 定义了函数mk_box，用于根据给定的边界创建一个立方体。
> 5. 定义了函数do_it，用于遍历实体，找出最小和最大边界，并创建立方体。
> 6. 定义了函数ufusr，这是NX二次开发的主入口函数，用于初始化和终止NX，并调用do_it。
> 7. 定义了函数ufusr_ask_unload，用于卸载用户函数库。
>
> 总的来说，这段代码通过遍历实体、确定边界、创建立方体的流程，实现了在NX中创建一个与当前部件边界相匹配的黄色立方体的功能。
>
