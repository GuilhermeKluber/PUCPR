import sys
import os
import cv2
from keras.models import load_model
import numpy as np
from src.utils.datasets import get_labels
from src.utils.inference import draw_text
from src.utils.inference import detect_faces
from src.utils.inference import draw_bounding_box
from src.utils.inference import apply_offsets
from src.utils.inference import load_detection_model
from src.utils.inference import load_image
from src.utils.preprocessor import preprocess_input

class FaceDetect(object):
    def __init__(self,model_detection):
        self.model_detection = model_detection
        self.face_detection  = load_detection_model(self.model_detection)

    def detect(self,img):
        #loading images
        gray_image = cv2.cvtColor(img.copy(),cv2.cv2.COLOR_BGR2GRAY)
        gray_image = np.squeeze(gray_image)
        gray_image = gray_image.astype('uint8')
        faces = detect_faces(self.face_detection, gray_image)
        return faces

class FaceAnalysis(object):
    def __init__(self,model_emotion,emotion_label):
        self.model_emotion  = model_emotion
        self.emotion_label = emotion_label
        #parameters for loading data and images
        font = cv2.FONT_HERSHEY_SIMPLEX
        # hyper-parameters for bounding boxes shape
        self.emotion_offsets = (20, 40)
        self.emotion_offsets = (0, 0)
        #loading models
        self.emotion_classifier = load_model(self.model_emotion, compile=False)

    def predict(self,img, faces):
        analysis = []

        #getting input model shapes for inference
        emotion_target_size = self.emotion_classifier.input_shape[1:3]
        #rgb_image = load_image(img , grayscale=False)
        rgb_image = cv2.cvtColor(img.copy(),cv2.cv2.COLOR_BGR2RGB)
        #gray_image = load_image(img , grayscale=True)
        gray_image = cv2.cvtColor(img.copy(),cv2.cv2.COLOR_BGR2GRAY)
        gray_image = np.squeeze(gray_image)
        gray_image = gray_image.astype('uint8')

        for face_coordinates in faces:
            x1, x2, y1, y2 = apply_offsets(face_coordinates, self.emotion_offsets)
            gray_face = gray_image[y1:y2, x1:x2]

            try:
                gray_face = cv2.resize(gray_face, (emotion_target_size))
            except:
                continue

            gray_face = preprocess_input(gray_face, True)
            gray_face = np.expand_dims(gray_face, 0)
            gray_face = np.expand_dims(gray_face, -1)
            emotion_label_arg = np.argmax(self.emotion_classifier.predict(gray_face))
            emotion_text = emotion_labels[emotion_label_arg]

            color = (0, 0, 255)
            draw_bounding_box(face_coordinates, rgb_image, color)
            draw_text(face_coordinates, rgb_image, emotion_text, color, 0, -50, 1, 2)
                #ouput
            analysis.append([face_coordinates, emotion_text]) #Essa será a saida, externamente decidiremos para onde vai
        bgr_image = cv2.cvtColor(rgb_image, cv2.COLOR_RGB2BGR)
        return analysis, bgr_image # Aqui ele retorna lista de faces detectadas com os labels de genero e emoção

detection_model_path = './trained_models/detection_models/haarcascade_frontalface_default.xml'
emotion_model_path = './trained_models/emotion_models/fer2013_mini_XCEPTION.102-0.66.hdf5'
emotion_labels = get_labels('fer2013')
cap = cv2.VideoCapture(0)
facedetector = FaceDetect(detection_model_path)
featuresdetector = FaceAnalysis(emotion_model_path,emotion_labels) #Estou colocando seus parametros como entrada
# frame = cv2.imread('505594930.jpg')
# faces = facedetector.detect(frame)
# outputs,img = featuresdetector.predict(frame,faces)# Esse é um exemplo de como funcionaria a predição de genero e humor usando class
# cv2.imwrite("test.jpg",img)
while(True):
    ret, frame = cap.read()
    faces = facedetector.detect(frame)
    outputs,img = featuresdetector.predict(frame,faces)# Esse é um exemplo de como funcionaria a predição de genero e humor usando class
    cv2.imshow('frame',img)
    if cv2.waitKey(1) == 27:
        break
cap.release()
cv2.destroyAllWindows()
