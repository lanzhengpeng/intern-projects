### 【2047】report curve states 报告曲线状态

#### 代码

```cpp
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    static void report_curve_states(int num_states, UF_CURVE_state_t *states)  
    {  
        int  
            ii;  
        WRITENZ(num_states);  
        for (ii = 0; ii < num_states; ii++)  
        {  
            WRITE_D(ii);  
            switch (states[ii].state_code)  
            {  
                case UF_CURVE_STATE_DEGENERACY:  
                    WRITE_D(UF_CURVE_STATE_DEGENERACY);  
                    if (states[ii].flag & 2) { WRITE("Along V "); }  
                    else { WRITE("Along U "); }  
                    if (states[ii].flag & 1) { WRITE("fixed "); }  
                    WRITE(" near ");  
                    WRITE3F(states[ii].value);  
                    break;  
                case UF_CURVE_STATE_G1_DISCONTINUITY:  
                    WRITE_D(UF_CURVE_STATE_G1_DISCONTINUITY);  
                    if (states[ii].flag & 2) { WRITE("Along V "); }  
                    else { WRITE("Along U "); }  
                    if (states[ii].flag & 1) { WRITE("smoothed to G1 "); }  
                    if (states[ii].flag & 4) { WRITE("smoothed to Cn "); }  
                    WRITE("At knot ");  
                    WRITE_F(states[ii].value[0]);  
                    break;  
                case UF_CURVE_STATE_KNOT_NONC0:  
                    WRITE_D(UF_CURVE_STATE_KNOT_NONC0);  
                    if (states[ii].flag & 2) { WRITE("Along V "); }  
                    else { WRITE("Along U "); }  
                    if (states[ii].flag & 1) { WRITE("fixed "); }  
                    WRITE("at knot ");  
                    WRITE_F(states[ii].value[0]);  
                    break;  
                case UF_CURVE_STATE_KNOT_MULT:  
                    WRITE_D(UF_CURVE_STATE_KNOT_MULT);  
                    if (states[ii].flag & 2) { WRITE("Along V "); }  
                    else { WRITE("Along U "); }  
                    if (states[ii].flag & 1) { WRITE("fixed "); }  
                    WRITE("at knot ");  
                    WRITE_F(states[ii].value[0]);  
                    break;  
                case UF_CURVE_STATE_KNOT_DECREASING:  
                    WRITE_D(UF_CURVE_STATE_KNOT_DECREASING);  
                    if (states[ii].flag & 2) { WRITE("Along V "); }  
                    else { WRITE("Along U "); }  
                    if (states[ii].flag & 1) { WRITE("fixed "); }  
                    WRITE("at knot ");  
                    WRITE_F(states[ii].value[0]);  
                    break;  
                case UF_CURVE_STATE_CLOSED:  
                    WRITE_D(UF_CURVE_STATE_CLOSED);  
                    if (states[ii].flag & 2) { WRITE("Along V "); }  
                    else { WRITE("Along U "); }  
                    WRITE("\n");  
                    break;  
                case UF_CURVE_STATE_PERIODIC:  
                    WRITE_D(UF_CURVE_STATE_PERIODIC);  
                    if (states[ii].flag & 2) { WRITE("Along V "); }  
                    else { WRITE("Along U "); }  
                    WRITE("\n");  
                    break;  
                case UF_CURVE_STATE_KNOT_TOOCLOSE:  
                    WRITE_D(UF_CURVE_STATE_KNOT_TOOCLOSE);  
                    if (states[ii].flag & 2) { WRITE("Along V "); }  
                    else { WRITE("Along U "); }  
                    if (states[ii].flag & 1) { WRITE("fixed "); }  
                    WRITE("at knot ");  
                    WRITE_F(states[ii].value[0]);  
                    break;  
                case UF_CURVE_STATE_NOT_TRIMMED:  
                    WRITE_D(UF_CURVE_STATE_PERIODIC);  
                    if (states[ii].flag & 2) { WRITE("Along V "); }  
                    else { WRITE("Along U "); }  
                    WRITE("\n");  
                    break;  
                case UF_CURVE_STATE_UNNORMALIZED:  
                    WRITE_D(UF_CURVE_STATE_UNNORMALIZED);  
                    if (states[ii].flag & 2) { WRITE("Along V factor "); }  
                    else { WRITE("Along U factor "); }  
                    WRITE_F(states[ii].value[0]);  
                    break;  
                case UF_CURVE_STATE_CLAMPED:  
                    WRITE_D(UF_CURVE_STATE_CLAMPED);  
                    WRITE("Knot value ");  
                    WRITE_F(states[ii].value[0]);  
                    WRITE("minimum distance change ");  
                    WRITE_F(states[ii].value[1]);  
                    WRITE("maximum angle change ");  
                    WRITE_F(states[ii].value[2]);  
                    break;  
                case UF_CURVE_STATE_SMOOTHED_TO_CN:  
                    WRITE_D(UF_CURVE_STATE_SMOOTHED_TO_CN);  
                    if (states[ii].flag & 2) { WRITE("Along V "); }  
                    else { WRITE("Along U "); }  
                    if (states[ii].flag & 1) { WRITE("smoothed to G1 "); }  
                    if (states[ii].flag & 4) { WRITE("smoothed to Cn "); }  
                    WRITE("At knot ");  
                    WRITE_F(states[ii].value[0]);  
                    break;  
                default:  
                    WRITE("Unknown state ");  
                    WRITE_D(states[ii].state_code);  
                    break;  
            }  
        }  
        if (num_states > 0) UF_free(states);  
    }

```

#### 代码解析

> 这段代码主要用于报告曲线的状态信息，具体包括以下功能：
>
> 1. 定义了几个宏，用于打开列表窗口并写入信息，包括整型、浮点数和浮点数数组的写入。
> 2. 定义了write_integer_to_listing_window、write_double_to_listing_window和write_double_array_to_listing_window函数，用于向列表窗口写入整型、浮点数和浮点数组。
> 3. 定义了report_curve_states函数，该函数接收曲线状态数组，遍历状态数组，并根据状态码打印不同类型的状态信息，包括退化、G1不连续、非C0结点、多重结点、递减结点、闭合、周期、过近结点、未修剪、未规范化、夹持和光滑到Cn等状态。
> 4. 在打印每种状态信息时，会输出状态码，并根据标志位输出沿U或V、固定、光滑等信息，同时输出对应的值，如结点位置、最小距离变化、最大角度变化等。
> 5. 在函数末尾，如果状态数大于0，释放状态数组内存。
> 6. 该代码利用宏简化了打开窗口和写入信息的操作，通过函数封装实现了不同类型数据的写入，通过switch-case结构实现了不同状态信息的打印，并通过标志位解析输出了状态详细信息，实现了对曲线状态的完整报告。
>
