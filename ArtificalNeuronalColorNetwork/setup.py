from setuptools import setup, find_packages

setup(
    name='ANNServer',
    version='1.0.4',
    packages=find_packages(),
    install_requires=[
        'joblib==1.3.2',
        'keras==2.15.0',
        'matplotlib==3.8.2',
        'numpy==1.26.3',
        'openpyxl==3.1.2',
        'pandas==2.2.0',
        'scikit-learn==1.4.0',
        'scipy==1.12.0',
        'tensorflow==2.15.0',
        'zipp==3.17.0'

    ],
    entry_points={
        'console_scripts': [
            'ANNServer = ANNServer.main:main',
            'TrainANNServer = ANNServer.main:train'
        ]
    },
    author='Lars Przybylek',
    author_email='l.p.1999@live.de',
    url='https://github.com/rclarsfull/ImageLight/tree/main/ArtificalNeuronalColorNetwork',
    license='GLP-3',
    include_package_data=True,
    package_data = {'': ['scaler_model.joblib', 'model.keras', 'data.xlsx']}
)
