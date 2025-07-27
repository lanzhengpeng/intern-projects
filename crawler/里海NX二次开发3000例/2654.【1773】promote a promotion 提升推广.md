### 【1773】promote a promotion 提升推广

#### 代码

```cpp
    /*HEAD PROMOTE_A_PROMOTION CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，我总结如下：

1. UF_print_syslog是V18版本中新增的函数。

2. 该函数用于打印系统日志。

3. 在使用时，需要传递日志信息作为参数。

4. 该函数将日志信息输出到系统日志文件中。

5. 使用该函数可以方便地记录程序运行时的关键信息，便于调试和监控程序运行状态。

6. 它可以记录不同级别的日志信息，如INFO、ERROR等。

7. UF_print_syslog函数的加入使得V18版本的日志功能更加完善。

以上是我对该函数的简要翻译和总结，如有需要补充或修改的地方，请告诉我。 */  
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
    static void do_it(void)  
    {  
        tag_t  
            a_body,  
            a_comp_in_b,  
            a_comp_in_c,  
            a_inst,  
            a_occ_in_b,  
            a_occ_in_c,  
            b_body,  
            b_comp_in_c,  
            b_inst,  
            c_body,  
            feat,  
            part;  
        double  
            abs_matrix[6] = { 1,0,0, 0,1,0 },  
            origin[3] = { 0,0,0 };  
        char  
            *sides[3] = { "3.0", "2.0", "1.0" };  
        UF_PART_load_status_t  
            status;  
        UF_CALL(UF_PART_new("a", UF_PART_ENGLISH, &part));  
        UF_CALL(UF_MODL_create_block1(UF_NULLSIGN, origin, sides, &feat));  
        UF_CALL(UF_MODL_ask_feat_body(feat, &a_body));  
        UF_CALL(UF_PART_new("b", UF_PART_ENGLISH, &part));  
        UF_CALL(UF_ASSEM_add_part_to_assembly(part, "a", "", "", origin,  
            abs_matrix, 0, &a_inst, &status));  
        a_comp_in_b = UF_ASSEM_ask_part_occ_of_inst(NULL_TAG, a_inst);  
        a_occ_in_b = UF_ASSEM_find_occurrence(a_comp_in_b, a_body);  
        UF_CALL(UF_MODL_create_promotion(a_occ_in_b, &feat));  
        UF_CALL(UF_MODL_ask_feat_body(feat, &b_body));  
        UF_CALL(UF_PART_new("c", UF_PART_ENGLISH, &part));  
        UF_CALL(UF_ASSEM_add_part_to_assembly(part, "b", "", "", origin,  
            abs_matrix, 0, &b_inst, &status));  
        b_comp_in_c = UF_ASSEM_ask_part_occ_of_inst(NULL_TAG, b_inst);  
        a_comp_in_c = UF_ASSEM_ask_part_occ_of_inst(b_comp_in_c, a_inst);  
        a_occ_in_c = UF_ASSEM_find_occurrence(a_comp_in_c, b_body);  
        UF_CALL(UF_MODL_create_promotion(a_occ_in_c, &feat));  
        UF_CALL(UF_MODL_ask_feat_body(feat, &c_body));  
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

> 这段代码是用于NX的二次开发，主要功能是实现零件的Promote操作。
>
> 主要步骤包括：
>
> 1. 创建两个零件a和b，并添加到装配中。
> 2. 在零件a中创建一个块特征，并获取其特征体。
> 3. 在零件b中，找到来自零件a的组件，并对该组件创建Promote特征，获取Promote后的特征体。
> 4. 重复上述步骤，在零件c中创建Promote特征。
> 5. 使用UF_print_syslog函数打印日志信息，用于调试。
> 6. 提供ufusr和ufusr_ask_unload两个标准接口。
>
> 通过这些步骤，实现了零件的Promote操作，并在调试时使用了UF_print_syslog函数打印日志信息。
>
