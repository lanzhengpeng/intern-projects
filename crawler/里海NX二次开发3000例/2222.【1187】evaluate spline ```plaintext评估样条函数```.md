### 【1187】evaluate spline ```plaintext评估样条函数```

#### 代码

```cpp
    #define WRITE_L(X) if (X) ECHO("%s = TRUE\n", #X); \  
        else ECHO("%s = FALSE\n", #X);  
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
    #define WRITEnF4(X,n) (write_double_array_to_listing_window(#X, X, n, 4))  
    #define WRITEnF(X,n) (write_double_array_to_listing_window(#X, X, n, 1))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1];  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (cols > 0)  
                if (ii == 0) ECHO("%s = ", name);  
                else ECHO("%s", blanks);  
            for (jj = 0; jj < cols; jj++)  
            {  
                ECHO("%f", array[kk++]);  
                if (jj < cols - 1) ECHO("%s", ", ");  
            }  
            if (cols > 0) ECHO("%s", "\n");  
        }  
    }  
    static void evaluate_spline(tag_t item)  
    {  
        logical  
            is_spline;  
        int  
            dimensionality,  
            ii,  
            n_knots,  
            n_points;  
        double  
            data[6],  
            *knots,  
            *control_pts;  
        UF_EVAL_p_t  
            spline_eval;  
        UF_EVAL_spline_t  
            spline;  
        UF_OBJ_disp_props_t  
            attrib = { 0, UF_OBJ_WHITE, 0, 0, 1, 0 };  
        if (!UF_CALL(UF_EVAL_initialize(item, &spline_eval)))  
        {  
            UF_CALL(UF_EVAL_is_spline(spline_eval, &is_spline));  
            if (is_spline)  
            {  
                UF_CALL(UF_EVAL_ask_spline(spline_eval, &spline));  
                WRITE_L(spline.is_periodic);  
                WRITE_L(spline.is_rational);  
                WRITE_D(spline.order);  
                WRITE_D(spline.num_knots);  
                UF_CALL(UF_EVAL_ask_spline_knots(spline_eval, &n_knots, &knots));  
                WRITEnF(knots, n_knots);  
                UF_free(knots);  
                WRITE_D(spline.num_control);  
                UF_CALL(UF_EVAL_ask_spline_control_pts(spline_eval, &n_points,  
                    &control_pts));  
                WRITEnF4(control_pts, n_points);  
                for (ii = 0; ii < n_points * 4; ii = ii + 4)  
                    UF_CALL(UF_DISP_display_temporary_point(NULL_TAG,  
                        UF_DISP_USE_ACTIVE_PLUS, &control_pts[ii], &attrib,  
                        UF_DISP_POINT));  
                UF_free(control_pts);  
            }  
            else  
                WRITE_L(is_spline);  
            UF_CALL(UF_EVAL_free(spline_eval));  
            UF_CALL(UF_MODL_ask_obj_dimensionality(item, &dimensionality, data));  
            WRITE_D(dimensionality);  
        }  
    }

```

#### 代码解析

> 这是段NX（Siemens NX）的二次开发代码，主要功能是获取和输出NX曲线对象是否为样条曲线，如果是样条曲线，则输出样条曲线的相关信息，包括是否为周期样条、有理样条，样条阶数、节点数、控制点数等。同时，代码还输出了曲线的维度信息。
>
> 具体来说，代码定义了一些宏，用于输出逻辑值和整数。接着定义了一个函数，用于将double数组输出到列表窗口。主函数evaluate_spline用于获取曲线对象，判断是否为样条曲线，如果是，则获取样条曲线的相关信息，并输出。样条曲线的信息包括是否为周期样条、有理样条，样条阶数、节点数、控制点数等。同时，代码还输出了曲线的维度信息。
>
> 这段代码主要用于NX的样条曲线相关信息的获取和输出，可以方便地了解样条曲线的详细情况，为后续处理提供基础。
>
