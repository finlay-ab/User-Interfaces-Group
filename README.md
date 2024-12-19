# Project Title: COMP2811 UI Final Project Coursework

## Overview

This application is designed to monitor and analyse water quality data, with a focus on pollutant tracking and compliance with safety standards. It was developed using **C++/Qt6** with a modular and user-friendly interface that adapts to changes in locale for basic internationalisation.

## Group Members

- Daniel Clayton (201896050)  
- Tayshan Cole (201700587)  
- Finlay Brydges (201759422)  
- Nadia Rahman (201734099)  
- Rameesah Farooqui (201643626)

## Getting Started

### Prerequisites

- **C++17** or higher  
- **Qt6** libraries, including:  
  - **QtWidgets**  
  - **QtCharts**

### Installation and Setup

1. **Extract the Starter Code**:  
- Unzip the provided starter code package to a working directory.  
    
2. **Build the Project**:  
   - cd cd cwk2\ solution/
     
   - Create a `build` directory and run `cmake`:  
       
     mkdir build && cd build  
       
     cmake ..  
       
   - Compile the project:  
       
     make  
       
   - Run the application:  
       
     ./quaketool

     
3. **Data Setup**:  
     
   - Place your CSV dataset in the specified directory (`/data` folder).  
   - Ensure that the application has access to the data file, and adjust file paths as necessary.

## Application Structure

### Main Components

- **Dashboard**: Provides an overview of the application’s main pages and core functionality.  
- **Data Page**: Displays pollutant data in a table format, allowing us to browse relevant fields. Utilises the model/view architecture from the starter code.  
- **Analysis Pages**:  
  - **Pollutant Trends**: Visualises changes in pollutant levels over time.  
  - **Safety Compliance**: Displays alerts for pollutant levels that exceed recommended limits.  
  - **Summary Statistics**: Calculates and displays averages, maximums, minimums, and other summary data.

## Key Features

- **Modular UI**: Using a "card" approach, each data type or control is grouped logically within the interface.  
- **Tooltips**: Hovering over elements displays additional context, improving usability.

## Design Choices

- **Model/View Architecture**: Chosen for flexibility in data handling and to separate data presentation from underlying storage.  
- **Data Visualisation**: Includes basic charts and compliance indicators to ensure intuitive data interpretation.  
- **"Card" Layout**: The UI is modularised into “cards,” each focused on a specific data group or control area.

## Included Files

- **Application Folder**: Folder that contains all source files and other components required to build the app.  
- **Report**: A PDF of the report documenting the app's development.  
- **Video Files** Videos documenting each of iteration of the application.  
- **Ethics Documentation**: Participant consent form and information sheet from user feedback gathering.

## Known Issues

- Backend implementation is not complete. Some pages must use their own dummy data to display their functionality.  
- Main dashboard function to switch between pages is not complete. Pages instead all pop up at once.

## Future Development

- Fixing of issues identified above (proper implementation of backend into all pages, proper implementation of page switching).  
- Improved implementation of compliance indicators to allow for more information to be displayed.  
- Adapting of application for a larger dataset (such as a water quality dataset for the entire UK) to allow for wider use of the application.  
- Implementation of more data pages to allow for a wider variety of information to be accessed (such as map views of pollution hotspots).

## Acknowledgments

- Environment Agency Water Quality Archive for providing the water quality data.

