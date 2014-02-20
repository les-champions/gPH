#include "EditorSettings.h"
#include "MainWindow.h"
#include "PH.h"
#include "PHIO.h"
#include <QtGui>
#include "Area.h"
#include "PH.h"
#include "PHIO.h"

EditorSettings::EditorSettings(PHPtr myPHPtr): QDialog()
{
<<<<<<< HEAD
    choiceBox = new QGroupBox("Exclusive checkBox");

    groupLayout = new QVBoxLayout;
=======

    groupLayout = new QVBoxLayout;
    choiceBox = new QGroupBox("Exclusive checkBox");

    // Get all the sorts of the PH file
    list<SortPtr> allSorts = myPHPtr->getSorts();
    for(SortPtr &s : allSorts){

       sort_Name = new QCheckBox(QString::fromStdString(s->getName()));
       tabSorts.push_back(sort_Name);
        // Insert the item at the end of the layout: sorts
        groupLayout->addWidget(sort_Name);
    }
>>>>>>> master

    QString chaine1, chaine2;
    int i=0;

    // Get all the sorts of the PH file
    list<SortPtr> allSorts = myPHPtr->getSorts();

    for(SortPtr &s : allSorts){
       sort_Name = new QCheckBox(QString::fromStdString(s->getName()));
        // Insert the item at the end of the layout: sorts
        groupLayout->addWidget(sort_Name);
        if(i==0)
            listehidenSorts.push_back(QString::fromStdString(s->getName()));
        i++;
    }
hideNonSelected(listehidenSorts,myPHPtr);

    //Buttons
    Generate = new QPushButton("&Generate");
    Annuler = new QPushButton("&Cancel");
    Edit = new QPushButton("&Edit");

    connect(Edit, SIGNAL(clicked()), this, SLOT( hideNonSelected(listehidenSorts,myPHPtr)));

    btnLayout = new QHBoxLayout;
    btnLayout->addWidget(Annuler);
    btnLayout->addWidget(Edit);
    btnLayout->addWidget(Generate);


    globalLayout = new QVBoxLayout;
    globalLayout->addLayout(groupLayout);
    globalLayout->addLayout(btnLayout);


    this->setLayout(globalLayout);

    //ajout du scroll
    widget = new QWidget;
    widget->setLayout(globalLayout);
    area = new QScrollArea;
    area->setWidget(widget);
    area->setWidgetResizable(true);

    layoutTotal = new QVBoxLayout;
    layoutTotal->addWidget(area);

    setLayout(layoutTotal);
    setWindowTitle("Choice of Sorts");
    setModal(true);
    resize(500,500);

}


<<<<<<< HEAD
 list<std::string> EditorSettings::getSelectedSorts(){

     list<std::string> vect=new list<std::string> ;

    for(QCheckBox &s : this->tabSorts){
        if(s.isChecked())
            vect.push_back(s.accessibleName());
    }
    return vect;
=======
void EditorSettings::hideNonSelected(QList<QString> listehidenSorts,PHPtr myPHPtr){

    for (QString &n: listehidenSorts){
        // Hide the QGraphicsItem representing the sort
        myPHPtr->getGraphicsScene()->getGSort(n.toStdString())->GSort::hide();

        // Hide all the actions related to the sort
        std::vector<GActionPtr> allActions = myPHPtr->getGraphicsScene()->getActions();
        for (GActionPtr &a: allActions){
            if (a->getAction()->getSource()->getSort()->getName() == n.toStdString() || a->getAction()->getTarget()->getSort()->getName() == n.toStdString() || a->getAction()->getResult()->getSort()->getName() == n.toStdString()){
                a->getDisplayItem()->hide();
            }
        }
        }
>>>>>>> 0857aebb2d538ff578b45dffe853d726f07d3120

}


<<<<<<< HEAD
    //Detroyer
    EditorSettings::~EditorSettings(){}
<<<<<<< HEAD
=======


>>>>>>> master
=======

    //Detroyer
    EditorSettings::~EditorSettings(){}

>>>>>>> 0857aebb2d538ff578b45dffe853d726f07d3120
