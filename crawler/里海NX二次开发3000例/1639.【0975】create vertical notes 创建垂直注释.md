### 【0975】create vertical notes 创建垂直注释

#### 代码

```cpp
    /*HEAD CREATE_VERTICAL_NOTES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_drf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：UF_print_syslog是在V18版本中新增的。 */  
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
        int  
            mpi[100];  
        tag_t  
            a_note;  
        double  
            origin[3] = { 0,0,0 },  
            mpr[70];  
        char  
            diameter[27],  
            *text[1],  
            radius[27];  
        UF_CALL(UF_DRF_ask_preferences(mpi, mpr, radius, diameter));  
        mpi[30] = 8;  /* Bottom center 里海译:Bottom center翻译为底部居中。 */  
        mpr[3] = 0.0; /* Text angle 里海译:Text angle */  
        UF_CALL(UF_DRF_set_preferences(mpi, mpr, radius, diameter));  
        text[0] = "VERTICAL NOTE";  
        UF_CALL(UF_DRF_create_note( 1, text, origin, 1, &a_note ));  
        mpi[30] = 4;  /* Mid left 里海译:Mid left的中文翻译为：中部偏左 */  
        mpr[3] = 90.0; /* Text angle 里海译:Text angle

角度

角度是几何学中的基本概念之一，用来描述两条射线或线段在相交点形成的夹角。在二维空间中，角度通常用来度量图形或图形间的相对位置关系。角度的度量单位有度、弧度等。在文本分析中，角度常用于描述文本行或字符间的倾斜程度。例如，通过计算文本行与水平线的夹角，可以判断文本是否倾斜。因此，在文本分析中，角度是一个重要的特征，有助于判断文本的排版格式和版面布局。 */  
        UF_CALL(UF_DRF_set_preferences(mpi, mpr, radius, diameter));  
        text[0] = "NOTE WITH 90 DEGREE TEXT ANGLE";  
        origin[0] = 1.0;  
        UF_CALL(UF_DRF_create_note( 1, text, origin, 0, &a_note ));  
        mpi[30] = 1;  /* Top left 里海译:根据上下文，"Top left" 的翻译是 "左上角"。 */  
        UF_CALL(UF_DRF_set_preferences(mpi, mpr, radius, diameter));  
        text[0] = "VERTICAL NOTE WITH 90 DEGREE TEXT ANGLE";  
        origin[0] = 2.0;  
        UF_CALL(UF_DRF_create_note( 1, text, origin, 1, &a_note ));  
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

> 这段代码是用于NX CAD系统创建垂直注释的二次开发代码，其主要功能包括：
>
> 1. 获取并设置注释的默认参数，如文本位置、文本角度等。
> 2. 创建三种不同样式的垂直注释：第一个注释位于原点，文本水平，注释内容为"VERTICAL NOTE"。第二个注释位于(1,0,0)，文本垂直，注释内容为"NOTE WITH 90 DEGREE TEXT ANGLE"。第三个注释位于(2,0,0)，文本垂直且倾斜90度，注释内容为"VERTICAL NOTE WITH 90 DEGREE TEXT ANGLE"。
> 3. 第一个注释位于原点，文本水平，注释内容为"VERTICAL NOTE"。
> 4. 第二个注释位于(1,0,0)，文本垂直，注释内容为"NOTE WITH 90 DEGREE TEXT ANGLE"。
> 5. 第三个注释位于(2,0,0)，文本垂直且倾斜90度，注释内容为"VERTICAL NOTE WITH 90 DEGREE TEXT ANGLE"。
> 6. 使用UF_DRF模块的函数来实现注释的创建，如UF_DRF_ask_preferences获取注释参数，UF_DRF_set_preferences设置注释参数，UF_DRF_create_note创建注释等。
> 7. 使用UF_print_syslog函数打印错误信息，用于调试。
> 8. 使用UF_initialize和UF_terminate初始化和终止NX系统。
> 9. 提供ufusr和ufusr_ask_unload函数，供NX调用。
>
> 总体来说，这段代码通过设置注释参数并调用注释创建函数，实现了不同样式的垂直注释的创建。
>
