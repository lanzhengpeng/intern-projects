### 【2166】report joint info 报告关节信息

#### 代码

```cpp
    /*HEAD REPORT_JOINT_INFO CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_mech.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，V18中新增了一个名为UF_print_syslog的函数。

这个函数用于打印系统日志信息。 */  
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
    static void report_double_array(char *name, double *array, int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        printf("%s\n", name);  
        for (ii = 0; ii < rows; ii++)  
        {  
            printf("\t");  
            for (jj = 0; jj < cols; jj++)  
            {  
                printf("%f", array[kk++]);  
                if (jj < cols - 1) printf(", ");  
            }  
            printf("\n");  
        }  
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
    static tag_t ask_next_joint(tag_t joint)  
    {  
        int  
            eighty_six = 86,  
            irc;  
        FTN(uf6010)(&joint, &eighty_six, &irc);  
        if (irc == 0) return joint;  
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
            joint = NULL_TAG,  
            mechanism = NULL_TAG,  
            matrix;  
        double  
            origins[6],  
            matrices[18];  
        while ((mechanism = ask_next_mechanism(mechanism)) != NULL_TAG)  
        {  
            report_object_name("mechanism", mechanism);  
            FTN(uf6002)(&mechanism, &irc);  
            while ((joint = ask_next_joint(joint)) != NULL_TAG)  
            {  
                report_object_name("joint", joint);  
                FTN(uf6012)(&joint, origins, &matrix, matrices, &irc);  
                if (irc == 0)  
                {  
                    report_double_array("origins", origins, 2, 3);  
                    ECHO(matrix);  
                    report_double_array("matrices", matrices, 6, 3);  
                }  
                else  
                    ECHO(irc);  
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

> 这段代码是用于NX CAD软件的二次开发，主要功能是遍历和报告当前活动装配中的机构和关节的信息。以下是该代码的主要功能：
>
> 1. 定义了错误报告函数report_error，用于在UF函数调用失败时打印错误信息。
> 2. 定义了报告双精度数组函数report_double_array，用于打印二维双精度数组。
> 3. 定义了获取下一个机构/关节函数ask_next_mechanism/ask_next_joint，用于遍历装配中的机构和关节。
> 4. 定义了报告对象名称函数report_object_name，用于打印机构/关节的名称和tag。
> 5. 主函数do_it用于遍历当前装配中的所有机构和关节，并报告其名称、原点矩阵和变换矩阵。
> 6. NX启动函数ufusr初始化和终止NX API，并调用do_it函数。
> 7. NX卸载函数ufusr_ask_unload用于在卸载用户函数时立即卸载。
>
> 总的来说，该代码的主要目的是遍历当前装配中的机构和关节，并报告它们的名称和位置信息，为用户提供模型分析的功能。
>
