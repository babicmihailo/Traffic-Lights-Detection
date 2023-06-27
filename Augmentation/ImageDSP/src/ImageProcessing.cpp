
#include "ImageProcessing.h"
#include "ColorSpaces.h"
#include "Augmentations.h"

#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <string>

int counter = 420;

void imageProcessingFun(
	const QString& prog_name,
	QImage& out_img,
	const QImage& in_img,
    const QVector<double>& params
) {
	int x_size = in_img.width();
	int y_size = in_img.height();


    if(prog_name == "Histogram"){
		// Create empty output image.
		out_img = QImage(x_size, y_size, in_img.format());
		
		// Create buffers for YUV image.
		uchar* Y_buff = new uchar[x_size*in_img.height()];
		char* U_buff = new char[x_size*in_img.height()/4];
		char* V_buff = new char[x_size*in_img.height()/4];

        // Convert input image to YUV420 image.

        RGBtoYUV420(in_img.bits(), x_size, y_size, Y_buff, U_buff, V_buff);
        int L = params[0];
        myAugmentation0(Y_buff, x_size, y_size, Y_buff, L);

		
		// Zero out U and V component.
        procesing_YUV420(Y_buff, U_buff, V_buff, x_size, y_size, 1, 1, 1);

		// Convert YUV image back to RGB.
        YUV420toRGB(Y_buff, U_buff, V_buff, x_size, y_size, out_img.bits());

		// Delete used memory buffers.
		delete[] Y_buff;
		delete[] U_buff;
		delete[] V_buff;
    }else if(prog_name == "Saturation"){
        double S = params[0];
		out_img = in_img;
        uchar* y_buff = new uchar[x_size*y_size];
        myAugmentation1(in_img.bits(), y_buff, x_size, y_size, out_img.bits(), S);
        delete[] y_buff;
    }else if(prog_name == "Rotation"){
        out_img = in_img;
        /* TO DO: Perform image rotation */
        double angle = params[0];
        myAugmentation2(in_img.bits(), x_size, y_size, out_img.bits(), x_size/2, y_size/2, angle);
    }else if(prog_name == "Occlusions"){
        out_img = in_img;
        /* TO DO: Perform image rotation */
        double probability = params[0];
        myAugmentation3(in_img.bits(), x_size, y_size, out_img.bits(), probability);
    }else if(prog_name == "ALL"){
            double saturation = 1.75;
            //double rotation_angle = 360;
            double probability = 0.001;

            // Create empty output image.
            out_img = QImage(x_size, y_size, in_img.format());
            QImage out_temp = out_img;
            // Create buffers for YUV image.
            uchar* Y_buff = new uchar[x_size*in_img.height()];
            char* U_buff = new char[x_size*in_img.height()/4];
            char* V_buff = new char[x_size*in_img.height()/4];

            // Convert input image to YUV420 image.
            RGBtoYUV420(in_img.bits(), x_size, y_size, Y_buff, U_buff, V_buff);

            myAugmentation0(Y_buff, x_size, y_size,Y_buff, 255);

            // Zero out U and V component.
            procesing_YUV420(Y_buff, U_buff, V_buff, x_size, y_size, 1, 1, 1);

            // Convert YUV image back to RGB.
            YUV420toRGB(Y_buff, U_buff, V_buff, x_size, y_size, out_temp.bits());

            // Delete used memory buffers.
            delete[] Y_buff;
            delete[] U_buff;
            delete[] V_buff;

            // Apply Saturation Augmentation.
            QImage out_temp2(x_size, y_size, out_temp.format());
            uchar* y_buff = new uchar[x_size*y_size];
            myAugmentation1(out_temp.bits(), y_buff, x_size, y_size, out_temp2.bits(), saturation);
            delete[] y_buff;

            int angle = rand() % 46; // random value between 0 and 45
            if (rand() % 2 == 0) {
                if (rand() % 2 == 0) { // randomly choose whether to add or subtract from 315
                    angle += 315;
                } else {
                    angle = 360 - angle;
                }
            } else {
                angle = 0;
            }


            // Apply Rotation Augmentation.
            QImage out_temp3(x_size, y_size, out_temp2.format());
            myAugmentation2(out_temp2.bits(), x_size, y_size, out_temp3.bits(), x_size/2, y_size/2, angle);

            // Apply Occlusions Augmentation.
            myAugmentation3(out_temp3.bits(), x_size, y_size, out_img.bits(), probability);

            QString input_filepath = QString::fromStdString("/home/mihailo/Desktop/ADOS_PZ1_Labeling_and_Augmentation/ImageDSP/images/IMG#" + std::to_string(counter) + ".jpeg");

            counter++;
            QImage in_img(input_filepath);
            QString input_filename = QFileInfo(input_filepath).fileName(); // Extract the filename from the input image path
            QString output_filepath = QDir::homePath() + "/Desktop/output/" + input_filename; // Construct the output file path
            out_img.save(output_filepath); // Save the output image to the specified file path


        }

}
