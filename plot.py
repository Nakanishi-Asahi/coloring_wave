import numpy as np
import cv2

def main():
    img_height = 400
    img_width = 1920
    img = np.zeros((img_height, img_width, 3))
    blue_param = 0.6
    orange_param = 0.9
    white_param = 1.2
    #[blue, orange, white]
    img_flag = np.zeros((img_height, img_width, 3))
    draw_height = int(img_height * 0.8)
    draw_width = int(img_width * 0.8)
    for i in range(draw_height):
        for j in range(draw_width):
            img[int((img_height - draw_height) / 2) + i, int((img_width - draw_width) / 2) + j] = [100, 100, 100]
    fs = 44100
    len = 10 * fs
    num = 512
    sample_num = int(len / num)
    index_width = draw_width / sample_num

    blue_file = open('draw_blue.txt', 'r')
    blue_datalist = blue_file.readlines()
    orange_file = open('draw_orange.txt', 'r')
    orange_datalist = orange_file.readlines()
    white_file = open('draw_white.txt', 'r')
    white_datalist = white_file.readlines()
    for i in range(sample_num):
        start_x = round(index_width * i)
        end_x = round(index_width * (i + 1)) - 1
        line_num = end_x - start_x + 1
        _, blue_value_str = blue_datalist[i].split(' ')
        blue_value = float(blue_value_str)
        blue_height = int(draw_height * blue_value * blue_param)
        for j in range(blue_height):
            for k in range(line_num):
                img_flag[int((img_height - draw_height) / 2) + int(draw_height / 2) - int(blue_height / 2) + j, int((img_width - draw_width) / 2) + start_x + k, 0] = 1

        _, orange_value_str = orange_datalist[i].split(' ')
        orange_value = float(orange_value_str)
        orange_height = int(draw_height * orange_value * orange_param)
        for j in range(orange_height):
            for k in range(line_num):
                img_flag[int((img_height - draw_height) / 2) + int(draw_height / 2) - int(orange_height / 2) + j, int((img_width - draw_width) / 2) + start_x + k, 1] = 1

        _, white_value_str = white_datalist[i].split(' ')
        white_value = float(white_value_str)
        white_height = int(draw_height * white_value * white_param)
        for j in range(white_height):
            for k in range(line_num):
                img_flag[int((img_height - draw_height) / 2) + int(draw_height / 2) - int(white_height / 2) + j, int((img_width - draw_width) / 2) + start_x + k, 2] = 1

    for i in range(img_height):
        for j in range(img_width):
            if img_flag[i, j, 0] == 0:
                if img_flag[i, j, 1] == 0:
                    if img_flag[i, j, 2] == 0:
                        img[i, j] += [0, 0, 0]
                    else:
                        img[i, j] = [255, 255, 255]
                elif img_flag[i, j, 1] == 1:
                    if img_flag[i, j, 2] == 0:
                        img[i, j] = [0, 159, 255]
                    else:
                        img[i, j] = [211, 239, 255]
            else:
                if img_flag[i, j, 1] == 0:
                    if img_flag[i, j, 2] == 0:
                        img[i, j] = [234, 87, 0]
                    else:
                        img[i, j] = [252, 219, 207]
                else:
                    if img_flag[i, j, 2] == 0:
                        img[i, j] = [0, 97, 191]
                    else:
                        img[i, j] = [212, 234, 247]
    cv2.imwrite('img.png', img)
    blue_file.close()

main()
