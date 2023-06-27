import cv2
from ultralytics import YOLO
import glob


model = YOLO('best380.pt')
#model = YOLO('best256.pt')


image_folder = 'images/'
image_files = glob.glob(image_folder + '*.jpeg')
images = []
for file_path in image_files:
    images.append(file_path)
video = "https://www.youtube.com/watch?v=JJhiIu8Xkfo"


def predict(src):
    results = model(src)

    for image_file in image_files:
        results = model(image_file)
        res_plotted = results[0].plot()

        cv2.imshow("result", res_plotted)
        cv2.waitKey(0)  # Wait until a key is pressed
        cv2.destroyAllWindows()


def track(src):
    results = model.track(source=src, conf=0.3, iou=0.5, show=True)


if __name__ == "__main__":
    try:
        predict(images)
        #track(video)
    except KeyboardInterrupt:
        pass


