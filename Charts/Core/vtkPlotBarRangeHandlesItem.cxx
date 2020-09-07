#include "vtkPlotBarRangeHandlesItem.h"

#include "vtkObjectFactory.h"
#include "vtkPlotBar.h"

//------------------------------------------------------------------------------
vtkStandardNewMacro(vtkPlotBarRangeHandlesItem);
vtkSetObjectImplementationMacro(vtkPlotBarRangeHandlesItem, PlotBar, vtkPlotBar);

void vtkPlotBarRangeHandlesItem::GetBounds(double bounds[4])
{
  if (!this->PlotBar)
  {
    vtkErrorMacro("vtkPlotBarRangeHandlesItem should always be used with a PlotBar");
    return;
  }

  double plotBounds[4];
  this->PlotBar->GetBounds(plotBounds);
  double range[2] = { plotBounds[0], plotBounds[1] };
  double length[2] = { plotBounds[2], plotBounds[3] };

  this->GetAxesUnscaledRange(range, length);

  this->TransformDataToScreen(range[0], length[0], bounds[0], bounds[2]);
  this->TransformDataToScreen(range[1], length[1], bounds[1], bounds[3]);
}

//------------------------------------------------------------------------------
vtkPlotBarRangeHandlesItem::vtkPlotBarRangeHandlesItem() {}

//------------------------------------------------------------------------------
vtkPlotBarRangeHandlesItem::~vtkPlotBarRangeHandlesItem() {}

//------------------------------------------------------------------------------
void vtkPlotBarRangeHandlesItem::SetActiveHandlePosition(double position)
{
  if (this->ActiveHandle != vtkPlotRangeHandlesItem::NO_HANDLE)
  {
    // Clamp the position and set the handle position
    double bounds[4];
    double clampedPos[2] = { position, 1 };
    this->GetBounds(bounds);

    double minRange = bounds[0];
    double maxRange = bounds[1];
    bounds[0] += this->HandleDelta;
    bounds[1] -= this->HandleDelta;

    vtkPlot::ClampPos(clampedPos, bounds);

    vtkVector2f point(clampedPos[0], clampedPos[1]);
    vtkVector2f tolerance(0.5, 0.5);
    vtkVector2f output;
    int plot = this->PlotBar->GetNearestPoint(point, tolerance, &output);
    float plotChartWidth = this->PlotBar->GetWidth();
    if (plot != -1)
    {
      this->ActiveHandlePosition = output[0] - 0.5 * plotChartWidth;
    }
    // Setup the position to the activeHandlePOsition in order to keep the 
    // handle in a limit of a bar
    position = this->ActiveHandlePosition;

    // Correct the position for range set
    if (this->ActiveHandle == vtkPlotRangeHandlesItem::LEFT_HANDLE)
    {
      position -= this->HandleDelta;
    }
    else
    {
      position += this->HandleDelta;
    }

    // Make the range value stick to the range for easier use
    if (minRange - this->HandleDelta <= position && position <= minRange + this->HandleDelta)
    {
      position = minRange;
    }
    if (maxRange - this->HandleDelta <= position && position <= maxRange + this->HandleDelta)
    {
      position = maxRange;
    }

    // Transform it to data and set it
    double unused;
    this->TransformScreenToData(position, 1, this->ActiveHandleRangeValue, unused);
  }
}
