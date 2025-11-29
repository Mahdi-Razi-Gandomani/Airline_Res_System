import torch
from pathlib import Path
from transformers import AutoTokenizer, AutoModel
import onnx

# Convert a sentence-transformers model to ONNX format
def convert_model_to_onnx(model_name: str, output_dir: str, opset_version: int = 14):

    output_path = Path(output_dir)
    output_path.mkdir(parents=True, exist_ok=True)
    

    tokenizer = AutoTokenizer.from_pretrained(model_name)
    model = AutoModel.from_pretrained(model_name)
    model.eval()
    
    # Save vocabulary
    tokenizer.save_vocabulary(str(output_path))
    
    # dummy input for export
    text = "This is a sample sentence for ONNX conversion"
    inputs = tokenizer(text, padding='max_length', max_length=128, truncation=True, return_tensors='pt')
    

    onnx_model_path = output_path / "sbert_model.onnx"

    
    with torch.no_grad():
        torch.onnx.export(model,
            (inputs['input_ids'], inputs['attention_mask']),
            str(onnx_model_path),
            input_names=['input_ids', 'attention_mask'],
            output_names=['last_hidden_state'],
            dynamic_axes={ 'input_ids': {0: 'batch', 1: 'sequence'}, 'attention_mask': {0: 'batch', 1: 'sequence'},
                'last_hidden_state': {0: 'batch', 1: 'sequence'}
            },
            opset_version=opset_version,
            do_constant_folding=True,
            verbose=False
        )
    

    onnx_model = onnx.load(str(onnx_model_path))
    onnx.save(onnx_model, str(onnx_model_path))
    




if __name__ == '__main__':

    convert_model_to_onnx('sentence-transformers/all-MiniLM-L6-v2', 'models', 14)