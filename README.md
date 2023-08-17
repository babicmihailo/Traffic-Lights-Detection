# YOLOv8 Traffic Light Tracking and Prediction

![GitHub License](https://img.shields.io/badge/license-MIT-blue.svg)

Welcome to the YOLOv8 Traffic Light Tracking and Prediction project! This project is built upon the YOLOv8 object detection framework and focuses on detecting, tracking, and predicting traffic lights in images, videos, and live streams. Additionally, the dataset is enhanced through image augmentations in C++ to increase detection reliability and performance.

## Features

- **YOLOv8 Framework**: The project utilizes the YOLOv8 architecture for accurate and efficient object detection, specifically focusing on traffic lights.

- **Traffic Light Detection**: The YOLOv8 model is trained to detect traffic lights in various scenarios, including real-world images, videos, and live streams.

- **Object Tracking**: Once detected, traffic lights are tracked across frames, enabling the monitoring of their movement and behavior.

- **Prediction**: The system predicts the behavior of detected traffic lights, aiding in traffic management and decision-making.

- **Image Augmentations**: The dataset is expanded using C++ image augmentations. This enhances the training data, leading to more reliable traffic light detection and prediction. Augmentations done are Saturation and Hue, Rotation & Occlusion.

## How to Use

1. **Setup**: Install the required dependencies and set up the YOLOv8 framework for object detection.

2. **Predict/Track**: Run the python script in predict/track mode 
   ```bash
   python3 main.py
   ```


