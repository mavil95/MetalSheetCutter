# Metal sheet cutter

<p>
Qt project on C++
</p>
<p>
This program places rectangles with custom size and quantity on the sheet of material with custom size and then calculate how much material will be spent.
</p>

## User Guide: 
1. Set size of sheet of materal
2. Set size and quantity of first and second group of objects
3. Press "Start cutting" button
4. To place objects to the materal sheet press "Place object" button

## Additional info:
- This program has an inaccuracy that increases as the size of the material sheet deviates from the size of the widget. The calculation will be as accurate as possible if the length or width of the material sheet is close to 500 cm.
- Input widgets has own value range. To check it hover the cursor over this widget

### TODO:
- Get rid of inaccuracies in measurements
- Add the ability to rotate objects when placing
- Adjust the logic of object placement - in some cases, unnecessary spaces appear
