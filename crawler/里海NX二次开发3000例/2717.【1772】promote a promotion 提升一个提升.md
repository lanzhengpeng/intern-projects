### 【1772】promote a promotion 提升一个提升

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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的说明，UF_print_syslog是在V18版本中新增的功能。所以，针对这个问题的答案是UF_print_syslog是V18版本新增的功能。 */  
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

> 这段NX二次开发代码实现了以下功能：
>
> 1. 创建了3个零件a、b、c。
> 2. 在零件a中创建了一个块特征。
> 3. 将零件a添加到装配体中，并在其上创建了一个提升特征。
> 4. 将零件b添加到装配体中，并在其上创建了另一个提升特征。
> 5. 在每个步骤中，通过调用UF函数实现了功能，并通过UF_CALL宏来报告错误。
> 6. do_it函数封装了上述功能。
> 7. ufusr函数是NX二次开发的入口函数，初始化后调用do_it，最后终止。
> 8. ufusr_ask_unload函数指定了二次开发模块卸载的方式。
> 9. 使用了NX的UF_PART、UF_MODL、UF_ASSEM等模块的API。
> 10. 通过宏和函数封装提高了代码的可读性和可维护性。
>
> 综上所述，这段代码实现了零件创建、装配体添加和提升特征创建等功能，展示了NX二次开发的基本流程。
>
