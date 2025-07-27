### 【2211】report modl states 报告模型状态

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
    static void report_modl_states(int num_states, UF_MODL_state_t *states)  
    {  
        int  
            ii;  
        WRITENZ(num_states);  
        for (ii = 0; ii < num_states; ii++)  
        {  
            WRITE_D(ii);  
            switch (states[ii].state_code)  
            {  
                case UF_MODL_STATE_DEGENERACY:  
                    WRITE_D(UF_MODL_STATE_DEGENERACY);  
                    if (states[ii].flag & 2) { WRITE("Along V "); }  
                    else { WRITE("Along U "); }  
                    if (states[ii].flag & 1) { WRITE("fixed "); }  
                    WRITE(" near ");  
                    WRITE3F(states[ii].value);  
                    break;  
                case UF_MODL_STATE_G1_DISCONTINUITY:  
                    WRITE_D(UF_MODL_STATE_G1_DISCONTINUITY);  
                    if (states[ii].flag & 2) { WRITE("Along V "); }  
                    else { WRITE("Along U "); }  
                    if (states[ii].flag & 1) { WRITE("smoothed to G1 "); }  
                    if (states[ii].flag & 4) { WRITE("smoothed to Cn "); }  
                    WRITE("At knot ");  
                    WRITE_F(states[ii].value[0]);  
                    break;  
                case UF_MODL_STATE_KNOT_NONC0:  
                    WRITE_D(UF_MODL_STATE_KNOT_NONC0);  
                    if (states[ii].flag & 2) { WRITE("Along V "); }  
                    else { WRITE("Along U "); }  
                    if (states[ii].flag & 1) { WRITE("fixed "); }  
                    WRITE("at knot ");  
                    WRITE_F(states[ii].value[0]);  
                    break;  
                case UF_MODL_STATE_KNOT_MULT:  
                    WRITE_D(UF_MODL_STATE_KNOT_MULT);  
                    if (states[ii].flag & 2) { WRITE("Along V "); }  
                    else { WRITE("Along U "); }  
                    if (states[ii].flag & 1) { WRITE("fixed "); }  
                    WRITE("at knot ");  
                    WRITE_F(states[ii].value[0]);  
                    break;  
                case UF_MODL_STATE_KNOT_DECREASING:  
                    WRITE_D(UF_MODL_STATE_KNOT_DECREASING);  
                    if (states[ii].flag & 2) { WRITE("Along V "); }  
                    else { WRITE("Along U "); }  
                    if (states[ii].flag & 1) { WRITE("fixed "); }  
                    WRITE("at knot ");  
                    WRITE_F(states[ii].value[0]);  
                    break;  
                case UF_MODL_STATE_CLOSED:  
                    WRITE_D(UF_MODL_STATE_CLOSED);  
                    if (states[ii].flag & 2) { WRITE("Along V "); }  
                    else { WRITE("Along U "); }  
                    WRITE("\n");  
                    break;  
                case UF_MODL_STATE_PERIODIC:  
                    WRITE_D(UF_MODL_STATE_PERIODIC);  
                    if (states[ii].flag & 2) { WRITE("Along V "); }  
                    else { WRITE("Along U "); }  
                    WRITE("\n");  
                    break;  
                case UF_MODL_STATE_KNOT_TOOCLOSE:  
                    WRITE_D(UF_MODL_STATE_KNOT_TOOCLOSE);  
                    if (states[ii].flag & 2) { WRITE("Along V "); }  
                    else { WRITE("Along U "); }  
                    if (states[ii].flag & 1) { WRITE("fixed "); }  
                    WRITE("at knot ");  
                    WRITE_F(states[ii].value[0]);  
                    break;  
                case UF_MODL_STATE_UNNORMALIZED:  
                    WRITE_D(UF_MODL_STATE_UNNORMALIZED);  
                    if (states[ii].flag & 2) { WRITE("Along V factor "); }  
                    else { WRITE("Along U factor "); }  
                    WRITE_F(states[ii].value[0]);  
                    break;  
                case UF_MODL_STATE_CLAMPED:  
                    WRITE_D(UF_MODL_STATE_CLAMPED);  
                    WRITE("Knot value ");  
                    WRITE_F(states[ii].value[0]);  
                    WRITE("minimum distance change ");  
                    WRITE_F(states[ii].value[1]);  
                    WRITE("maximum angle change ");  
                    WRITE_F(states[ii].value[2]);  
                    break;  
                case UF_MODL_STATE_SMOOTHED_TO_CN:  
                    WRITE_D(UF_MODL_STATE_SMOOTHED_TO_CN);  
                    if (states[ii].flag & 2) { WRITE("Along V "); }  
                    else { WRITE("Along U "); }  
                    if (states[ii].flag & 1) { WRITE("smoothed to G1 "); }  
                    if (states[ii].flag & 4) { WRITE("smoothed to Cn "); }  
                    WRITE("At knot ");  
                    WRITE_F(states[ii].value[0]);  
                    break;  
                case UF_MODL_STATE_DEGEN_EDGE:  
                    WRITE_D(UF_MODL_STATE_DEGEN_EDGE);  
                    break;  
                case UF_MODL_STATE_SELF_INTERSECT:  
                    WRITE_D(UF_MODL_STATE_SELF_INTERSECT);  
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

> 根据代码内容，这是一段用于报告NX中NURBS曲面状态的二次开发代码。
>
> 主要功能包括：
>
> 1. 定义了一系列宏，用于打开输出窗口、写入整数、浮点数、浮点数组等。
> 2. 实现了几个用于写入整数、浮点数、浮点数组的函数。
> 3. 定义了一个report_modl_states函数，用于报告NURBS曲面的各种状态信息，包括退化状态、G1不连续状态、非C0结点状态、多重结点状态、降阶结点状态、封闭状态、周期状态、过近结点状态、未归一化状态、夹持状态、平滑到Cn状态、退化边状态、自交状态等。
> 4. 在report_modl_states函数中，根据不同的状态码，使用宏写入不同的状态信息，并释放了状态数组内存。
> 5. 整个代码利用宏和函数，实现了对NX NURBS曲面状态的报告输出，方便用户查看和分析NURBS曲面存在的问题。
>
