/*
 * EventLoop.cpp
 *
 * Created: 26.05.2016 14:46:10
 *  Author: user
 */

#include "graphobj.hpp"
#include "../PlatformDrv.hpp"

 //Сигнал, по которому происходит перемещение фокуса ввода на следующий объект
const signal NextObjectSignal = signal::BtnDelUp;			//Отпускание кнопки Del

void PushSignal(const signal sig);

void GuiEventLoop() {
	layer *CurLayer = layer::layer_stack.pick();
	GraphObj *SelectedObject = CurLayer->head, *found;
	signal sig;

	for (found = SelectedObject;;) {				//ищем первый объект, который получит фокус ввода
		if (found->CanReciveFocus) {
			found->SetFocus();
			break;
		}
		found = found->next;
		if (found == nullptr) {
			found = CurLayer->head;
		}
	}

	while (1) {
		switch ((sig = WaitSignal())) {
			default:
				if (SelectedObject != nullptr && SelectedObject->CanReciveFocus && SelectedObject->Enabled) {
					SelectedObject->ProcSignal(sig);
				}
				if (!SelectedObject->Enabled) {		//На случай, если выделенный объект стал неактивным передадим фокус другому
					PushSignal(NextObjectSignal);
				}
				break;

			case NextObjectSignal:
				for (found = SelectedObject;;) {				//ищем следующий объект, который получит фокус ввода
					found = found->next;
					if (found == nullptr) {
						found = CurLayer->head;
					}
					if ((found->CanReciveFocus && found->Enabled) || found == SelectedObject) {
						break;
					}
				}
				if (found != SelectedObject) {		//Если найденный объект не тот же самый, что уже выделен
					SelectedObject->RemoveFocus();
					SelectedObject = found;
					SelectedObject->SetFocus();
				}
				break;

			case signal::BreakGuiLoop:
				if (CurLayer->prev != nullptr) {		//Нельзя остановить цикл на самом нижнем слое, выходить некуда
					return;
				}
		}
	}
}

void GuiEventLoop_noblock(VoidFuncPtr userHook) {
	layer *CurLayer = layer::layer_stack.pick();
	GraphObj *SelectedObject = CurLayer->head, *found;
	signal sig;

	while (1) {
		switch ((sig = GetSignal())) {			//Используем неблокирующую функцию для получения событий
			default:
				if (SelectedObject != nullptr && SelectedObject->CanReciveFocus && SelectedObject->Enabled && sig != signal::none) {
					SelectedObject->ProcSignal(sig);
				}
				break;

			case NextObjectSignal:
				for (found = SelectedObject;;) {
					found = found->next;
					if (found == nullptr) {
						found = CurLayer->head;
					}
					if ((found->CanReciveFocus && found->Enabled) || found == SelectedObject) {
						break;
					}
				}
				if (found != SelectedObject) {
					SelectedObject->RemoveFocus();
					SelectedObject = found;
					SelectedObject->SetFocus();
				}
				break;

			case signal::BreakGuiLoop:
				if (CurLayer->prev != nullptr) {
					return;
				}
			}
			if (userHook != nullptr) {
				userHook();
			}
	}
}
