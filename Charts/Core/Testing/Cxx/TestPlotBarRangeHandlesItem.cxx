#include <iostream>
#include <map>

#include <vtkAxis.h>
#include <vtkChartXY.h>
#include <vtkContextInteractorStyle.h>
#include <vtkContextScene.h>
#include <vtkContextView.h>
#include <vtkFloatArray.h>
#include <vtkIntArray.h>
#include <vtkInteractorEventRecorder.h>
#include <vtkNew.h>
#include <vtkPlotBar.h>
#include <vtkPlotBarRangeHandlesItem.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkTable.h>

//------------------------------------------------------------------------------
class vtkRangeHandlesCallBack : public vtkCommand
{
public:
  static vtkRangeHandlesCallBack* New() { return new vtkRangeHandlesCallBack; }

  void Execute(vtkObject* caller, unsigned long event, void* vtkNotUsed(callData)) override
  {
    vtkPlotRangeHandlesItem* self = vtkPlotRangeHandlesItem::SafeDownCast(caller);
    if (!self)
    {
      return;
    }
    if (event == vtkCommand::EndInteractionEvent)
    {
      self->GetHandlesRange(this->Range);
    }
    if (this->EventSpy.count(event) == 0)
    {
      this->EventSpy[event] = 0;
    }
    ++this->EventSpy[event];
    std::cout << "InvokedEvent: " << event << this->EventSpy[event] << std::endl;
  }
  std::map<unsigned long, int> EventSpy;
  double Range[2];
};

const int num_months = 12;

static const int month[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };

static const int book_2008[] = { 5675, 5902, 6388, 5990, 5575, 7393, 9878, 8082, 6417, 5946, 5526,
  5166 };

static void build_array(const char* name, vtkIntArray* array, const int c_array[])
{
  array->SetName(name);
  for (int i = 0; i < num_months; ++i)
  {
    array->InsertNextValue(c_array[i]);
  }
}

int TestPlotBarRangeHandlesItem(int, char*[])
{
  // Set up a 2D scene, add an XY chart to it
  vtkNew<vtkContextView> view;
  view->GetRenderWindow()->SetSize(500, 350);
  vtkNew<vtkChartXY> chart;
  view->GetScene()->AddItem(chart);

  vtkNew<vtkPlotBarRangeHandlesItem> rangeItem;
  chart->AddPlot(rangeItem);
  chart->RaisePlot(rangeItem);

  // Create a table with some points in it...
  vtkNew<vtkTable> table;

  vtkNew<vtkIntArray> arrMonth;
  build_array("Month", arrMonth, month);
  table->AddColumn(arrMonth);

  vtkNew<vtkIntArray> arrBooks2008;
  build_array("Books 2008", arrBooks2008, book_2008);
  table->AddColumn(arrBooks2008);

  vtkPlotBar* bar1 = vtkPlotBar::SafeDownCast(chart->AddPlot(vtkChart::BAR));
  rangeItem->SetPlotBar(bar1);
  bar1->SetInputData(table, "Month", "Books 2008");

  // Setup scene/interactor

  vtkNew<vtkContextScene> scene;
  scene->AddItem(rangeItem);

  vtkNew<vtkContextInteractorStyle> interactorStyle;
  interactorStyle->SetScene(scene);

  vtkNew<vtkRenderWindowInteractor> iren;
  iren->SetInteractorStyle(interactorStyle);

  vtkNew<vtkInteractorEventRecorder> recorder;
  recorder->SetInteractor(iren);
  recorder->ReadFromInputStringOn();

  vtkNew<vtkRangeHandlesCallBack> cbk;
  rangeItem->AddObserver(vtkCommand::StartInteractionEvent, cbk);
  rangeItem->AddObserver(vtkCommand::InteractionEvent, cbk);
  rangeItem->AddObserver(vtkCommand::EndInteractionEvent, cbk);

  //
  // Check initialization
  //

  double range[2];
  rangeItem->GetHandlesRange(range);

  if (range[0] != 0 || range[1] != 0)
  {
    std::cerr << "Initialization: Unexepected range in vertical range handle : [" << range[0]
              << ", " << range[1] << "]. Expecting : [0, 0]." << std::endl;
    return EXIT_FAILURE;
  }

  //
  // Check when moving right handle
  //

  const char rightEvents[] = "# StreamVersion 1\n"
                             "LeftButtonPressEvent 0 10 0 0 0 0 0\n"
                             "MouseMoveEvent 4 10 0 0 0 0 0\n"
                             "LeftButtonReleaseEvent 4 10 0 0 0 0 0\n";
  recorder->SetInputString(rightEvents);
  recorder->Play();

  if (cbk->EventSpy[vtkCommand::StartInteractionEvent] != 1 ||
    cbk->EventSpy[vtkCommand::InteractionEvent] != 1 ||
    cbk->EventSpy[vtkCommand::EndInteractionEvent] != 1)
  {
    std::cerr << "Move right handle: Wrong number of fired events : "
              << cbk->EventSpy[vtkCommand::StartInteractionEvent] << " "
              << cbk->EventSpy[vtkCommand::InteractionEvent] << " "
              << cbk->EventSpy[vtkCommand::EndInteractionEvent] << std::endl;
    return EXIT_FAILURE;
  }

  rangeItem->GetHandlesRange(range);
  if (range[0] != 0 || range[1] != 3.5)
  {
    std::cerr << "Unexepected range in vertical range handle : [" << range[0] << ", " << range[1]
              << "]. Expecting : [0, 3.5]." << std::endl;
    return EXIT_FAILURE;
  }

  //
  // Check when moving left handle
  //

  const char leftEvents[] = "# StreamVersion 1\n"
                             "LeftButtonPressEvent 0 10 0 0 0 0 0\n"
                             "MouseMoveEvent 2.17 10 0 0 0 0 0\n"
                             "LeftButtonReleaseEvent 2.17 10 0 0 0 0 0\n";
  recorder->SetInputString(leftEvents);
  recorder->Play();

  if (cbk->EventSpy[vtkCommand::StartInteractionEvent] != 1 ||
    cbk->EventSpy[vtkCommand::InteractionEvent] != 1 ||
    cbk->EventSpy[vtkCommand::EndInteractionEvent] != 1)
  {
    std::cerr << "Move left handle: Wrong number of fired events : "
              << cbk->EventSpy[vtkCommand::StartInteractionEvent] << " "
              << cbk->EventSpy[vtkCommand::InteractionEvent] << " "
              << cbk->EventSpy[vtkCommand::EndInteractionEvent] << std::endl;
    return EXIT_FAILURE;
  }

  rangeItem->GetHandlesRange(range);
  if (range[0] != 1.6 || range[1] != 3.5)
  {
    std::cerr << "Unexepected range in vertical range handle : [" << range[0] << ", " << range[1]
              << "]. Expecting : [1.6, 3.5]." << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}